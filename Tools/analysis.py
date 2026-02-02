import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.widgets import CheckButtons, TextBox, Button
from scipy.signal import savgol_filter
from scipy.ndimage import uniform_filter1d
from scipy.optimize import curve_fit

# ========= config =========
CSV_PATH = "./Tools/vofa2+.csv"
SPEED_COLUMN = "Encoder Parse Speed"  # column to apply better smoothing
SMOOTH_WINDOW = 15                    # window size for smoothing (odd number)
SMOOTH_POLY_ORDER = 2                 # polynomial order for Savitzky-Golay
# ==========================

df = pd.read_csv(CSV_PATH)
x = np.arange(len(df), dtype=float)

# ---- Speed smoothing options ----
def smooth_speed(y, method="savgol"):
    """
    Smooth noisy speed data using various methods.
    Methods: 'savgol', 'moving_avg', 'exp_weighted'
    """
    mask = np.isfinite(y)
    y_clean = y.copy()

    if method == "savgol":
        # Savitzky-Golay: best for preserving peaks/features while smoothing
        window = min(SMOOTH_WINDOW, len(y) // 4)
        if window % 2 == 0:
            window += 1  # must be odd
        window = max(window, 5)
        y_clean[mask] = savgol_filter(y[mask], window, SMOOTH_POLY_ORDER)

    elif method == "moving_avg":
        # Simple moving average
        y_clean[mask] = uniform_filter1d(y[mask], size=SMOOTH_WINDOW)

    elif method == "exp_weighted":
        # Exponential weighted moving average
        alpha = 0.2
        result = np.zeros_like(y)
        result[0] = y[0]
        for i in range(1, len(y)):
            if mask[i]:
                result[i] = alpha * y[i] + (1 - alpha) * result[i-1]
            else:
                result[i] = result[i-1]
        y_clean = result

    return y_clean

# ---- Prepare data with smoothed speed ----
plot_data = {}
for col in df.columns:
    y = df[col].to_numpy(dtype=float)
    plot_data[col] = y

# Add smoothed speed as separate trace
if SPEED_COLUMN in df.columns:
    raw_speed = df[SPEED_COLUMN].to_numpy(dtype=float)
    plot_data[f"{SPEED_COLUMN} (smoothed)"] = smooth_speed(raw_speed, "savgol")

# ---- Polynomial curve fitting for speed ----
def fit_polynomial(x, y, degree):
    """Fit polynomial and return fitted y values and equation string."""
    # Only fit on non-zero data (skip initial zeros)
    nonzero_mask = y != 0
    if not nonzero_mask.any():
        return None, None, None

    x_fit = x[nonzero_mask]
    y_fit = y[nonzero_mask]

    coefs = np.polyfit(x_fit, y_fit, degree)
    poly = np.poly1d(coefs)

    # Calculate R²
    y_pred = poly(x_fit)
    ss_res = np.sum((y_fit - y_pred) ** 2)
    ss_tot = np.sum((y_fit - np.mean(y_fit)) ** 2)
    r2 = 1 - ss_res / ss_tot if ss_tot > 0 else 0

    # Generate fitted curve for full x range
    y_fitted = poly(x)
    # Set to NaN where original was zero (to not show fit in zero region)
    y_fitted[~nonzero_mask] = np.nan

    # Build equation string (with explicit * for parsing)
    terms = []
    for i, c in enumerate(coefs):
        power = degree - i
        if abs(c) < 1e-12:
            continue
        if power == 0:
            terms.append(f"{c:.4g}")
        elif power == 1:
            terms.append(f"{c:.4g}*x")
        else:
            terms.append(f"{c:.4g}*x^{power}")
    equation = "y = " + " + ".join(terms).replace("+ -", "- ")

    return y_fitted, equation, r2

# Add polynomial fits for speed (degrees 1-8)
poly_fits = {}
if SPEED_COLUMN in df.columns:
    raw_speed = df[SPEED_COLUMN].to_numpy(dtype=float)
    print("\n=== Speed Polynomial Fits ===")
    for deg in [1, 2, 3, 4, 5, 6, 7, 8]:
        y_fit, eq, r2 = fit_polynomial(x, raw_speed, deg)
        if y_fit is not None:
            label = f"Speed fit (deg {deg})"
            plot_data[label] = y_fit
            poly_fits[label] = {"equation": eq, "r2": r2, "degree": deg}
            print(f"Degree {deg}: R²={r2:.6f}")
            print(f"  {eq}")

# ---- Second-order approximation of 8th order fit (for control systems) ----
def approximate_high_order_with_low(x, y_high, target_degree=2):
    """
    Fit a lower-order polynomial to approximate a higher-order fitted curve.
    This gives a simpler model that captures the essential behavior.
    """
    mask = np.isfinite(y_high)
    if not mask.any():
        return None, None, None

    x_valid = x[mask]
    y_valid = y_high[mask]

    coefs = np.polyfit(x_valid, y_valid, target_degree)
    poly = np.poly1d(coefs)

    # R² against the high-order fit (how well does 2nd order approximate 8th order)
    y_pred = poly(x_valid)
    ss_res = np.sum((y_valid - y_pred) ** 2)
    ss_tot = np.sum((y_valid - np.mean(y_valid)) ** 2)
    r2 = 1 - ss_res / ss_tot if ss_tot > 0 else 0

    y_approx = np.full_like(x, np.nan)
    y_approx[mask] = poly(x[mask])

    # Build equation (with explicit * for parsing)
    terms = []
    for i, c in enumerate(coefs):
        power = target_degree - i
        if abs(c) < 1e-12:
            continue
        if power == 0:
            terms.append(f"{c:.6g}")
        elif power == 1:
            terms.append(f"{c:.6g}*x")
        else:
            terms.append(f"{c:.6g}*x^{power}")
    equation = "y = " + " + ".join(terms).replace("+ -", "- ")

    return y_approx, equation, r2, coefs

# Generate suggested 2nd order approximation - fit directly to RAW speed data (more useful)
suggested_2nd_order = None
if SPEED_COLUMN in df.columns:
    raw_speed = df[SPEED_COLUMN].to_numpy(dtype=float)
    nonzero = raw_speed != 0

    if nonzero.any():
        x_nz = x[nonzero]
        y_nz = raw_speed[nonzero]

        # Normalize x to [0,1] range for better numerical stability
        x_min, x_max = x_nz.min(), x_nz.max()
        x_norm = (x_nz - x_min) / (x_max - x_min)  # normalized to [0,1]

        # Fit 2nd order on normalized x
        coefs_norm = np.polyfit(x_norm, y_nz, 2)
        poly_norm = np.poly1d(coefs_norm)

        # Calculate R² vs raw data
        y_pred = poly_norm(x_norm)
        ss_res = np.sum((y_nz - y_pred) ** 2)
        ss_tot = np.sum((y_nz - np.mean(y_nz)) ** 2)
        r2_approx = 1 - ss_res / ss_tot if ss_tot > 0 else 0

        # Generate curve for plot (using normalized x)
        y_approx = np.full_like(x, np.nan)
        x_full_norm = (x - x_min) / (x_max - x_min)
        y_approx[nonzero] = poly_norm(x_full_norm[nonzero])

        # Build equation string for NORMALIZED x (t = (x-x_min)/(x_max-x_min))
        a, b, c = coefs_norm
        eq_norm = f"y = {a:.6g}*t^2 + {b:.6g}*t + {c:.6g}".replace("+ -", "- ")
        eq_with_t = f"where t = (x - {x_min:.0f}) / {x_max - x_min:.0f}"

        # Also create equation in original x coordinates for direct use
        # y = a*((x-x_min)/range)^2 + b*((x-x_min)/range) + c
        # Expand: let range = x_max - x_min
        range_x = x_max - x_min
        a_orig = a / (range_x ** 2)
        b_orig = b / range_x - 2 * a * x_min / (range_x ** 2)
        c_orig = c + a * (x_min ** 2) / (range_x ** 2) - b * x_min / range_x

        eq_orig = f"y = {a_orig:.6g}*x^2 + {b_orig:.6g}*x + {c_orig:.6g}".replace("+ -", "- ")

        label = "2nd order approx (from deg 8)"
        plot_data[label] = y_approx
        poly_fits[label] = {"equation": eq_orig, "r2": r2_approx, "degree": 2, "coefs": [a_orig, b_orig, c_orig]}
        suggested_2nd_order = {"equation": eq_orig, "r2": r2_approx, "coefs": [a_orig, b_orig, c_orig],
                               "eq_normalized": eq_norm, "coefs_normalized": coefs_norm}

        print("\n" + "="*60)
        print("=== SUGGESTED 2nd ORDER APPROXIMATION (Control System) ===")
        print("="*60)
        print(f"Fit directly to raw speed data (R² = {r2_approx:.6f}):")
        print(f"\nNORMALIZED form (t in [0,1], better for control):")
        print(f"  {eq_norm}")
        print(f"  {eq_with_t}")
        print(f"  Coefficients: a={a:.6g}, b={b:.6g}, c={c:.6g}")
        print(f"\nORIGINAL x form (for plotting):")
        print(f"  {eq_orig}")
        print("="*60)

# ---- Print speed stats ----
if SPEED_COLUMN in df.columns:
    raw = df[SPEED_COLUMN].to_numpy(dtype=float)
    smoothed = plot_data[f"{SPEED_COLUMN} (smoothed)"]
    nonzero_mask = raw != 0
    if nonzero_mask.any():
        print(f"\n=== Speed Analysis ===")
        print(f"Raw speed std dev:      {np.std(raw[nonzero_mask]):.4f}")
        print(f"Smoothed speed std dev: {np.std(smoothed[nonzero_mask]):.4f}")
        print(f"Raw speed mean:         {np.mean(raw[nonzero_mask]):.4f}")
        print(f"Smoothed speed mean:    {np.mean(smoothed[nonzero_mask]):.4f}")

# ---- Plot all on ONE graph with toggle checkboxes ----
fig, ax = plt.subplots(figsize=(14, 8))
plt.subplots_adjust(left=0.28)  # make room for checkboxes

lines = {}
labels = list(plot_data.keys())

# Color scheme: different colors for data, fits get distinct dashed styles
fit_styles = {
    "Speed fit (deg 1)": {"color": "#e41a1c", "linestyle": "--", "linewidth": 2},
    "Speed fit (deg 2)": {"color": "#377eb8", "linestyle": "-.", "linewidth": 2},
    "Speed fit (deg 3)": {"color": "#4daf4a", "linestyle": ":", "linewidth": 2.5},
    "Speed fit (deg 4)": {"color": "#984ea3", "linestyle": "-", "linewidth": 2},
    "Speed fit (deg 5)": {"color": "#ff7f00", "linestyle": "--", "linewidth": 2},
    "Speed fit (deg 6)": {"color": "#a65628", "linestyle": "-.", "linewidth": 2},
    "Speed fit (deg 7)": {"color": "#f781bf", "linestyle": ":", "linewidth": 2.5},
    "Speed fit (deg 8)": {"color": "#00ced1", "linestyle": "-", "linewidth": 2},
    "2nd order approx (from deg 8)": {"color": "#ff1493", "linestyle": "-", "linewidth": 3},
    "Custom curve": {"color": "#000000", "linestyle": "-", "linewidth": 2.5},
}

# Assign colors for non-fit data
base_colors = plt.cm.tab10(np.linspace(0, 1, len(labels)))

for i, label in enumerate(labels):
    y = plot_data[label]

    if label in fit_styles:
        style = fit_styles[label]
        # Add R² to legend label
        if label in poly_fits:
            r2 = poly_fits[label]["r2"]
            legend_label = f"{label} (R²={r2:.4f})"
        else:
            legend_label = label
        line, = ax.plot(x, y, label=legend_label, **style)
    elif "smoothed" in label:
        line, = ax.plot(x, y, label=label, color="orange", linewidth=2, alpha=0.8)
    else:
        line, = ax.plot(x, y, label=label, color=base_colors[i], linewidth=1, alpha=0.7)

    lines[label] = line

# Add placeholder for custom curve (initially invisible)
custom_line, = ax.plot([], [], label="Custom curve", color="#000000", linewidth=2.5)
custom_line.set_visible(False)
lines["Custom curve"] = custom_line

ax.set_xlabel("Sample index")
ax.set_ylabel("Value")
ax.set_title("All Signals (click checkboxes to toggle) - Polynomial fits shown with R²")
ax.grid(True, alpha=0.3)
ax.legend(loc="upper right", fontsize=7)

# ---- Checkbox widget for toggling visibility ----
# Initial visibility: show key signals + smoothed, hide poly fits by default
default_visible = {
    "Encoder Read Distance", "Target Distance", SPEED_COLUMN,
    f"{SPEED_COLUMN} (smoothed)", "target Speed", "PWM Output", "Error"
}

# Add "Custom curve" to the labels for checkbox
all_labels = labels + ["Custom curve"]

for label, line in lines.items():
    line.set_visible(label in default_visible)

# Create checkboxes
rax = plt.axes([0.01, 0.20, 0.22, 0.65])
check = CheckButtons(rax, all_labels, [label in default_visible for label in all_labels])

# Style checkboxes - color them to match lines
for i, (label_text, label) in enumerate(zip(check.labels, all_labels)):
    label_text.set_fontsize(7)
    if label in fit_styles:
        label_text.set_color(fit_styles[label]["color"])
        label_text.set_fontweight("bold")
    elif "smoothed" in label:
        label_text.set_color("orange")
        label_text.set_fontweight("bold")

def toggle_visibility(label):
    if label in lines:
        lines[label].set_visible(not lines[label].get_visible())
        ax.legend(loc="upper right", fontsize=7)
        fig.canvas.draw_idle()

check.on_clicked(toggle_visibility)

# ---- Text box for custom equation input ----
# Get non-zero x range for reference
nonzero_mask = df[SPEED_COLUMN].to_numpy() != 0 if SPEED_COLUMN in df.columns else np.ones(len(x), dtype=bool)
x_min_nz, x_max_nz = x[nonzero_mask].min(), x[nonzero_mask].max()

# Add text box - use simple constant as default (user can paste from console)
textbox_ax = plt.axes([0.01, 0.08, 0.22, 0.04])
# Default to mean speed value for easier testing
if SPEED_COLUMN in df.columns:
    mean_speed = df[SPEED_COLUMN][df[SPEED_COLUMN] != 0].mean()
    initial_eq = f"y = {mean_speed:.2f}"
else:
    initial_eq = "y = 60"
textbox = TextBox(textbox_ax, '', initial=initial_eq)
textbox_ax.set_title("Custom Equation (use 'x' as variable):", fontsize=8, loc='left')

# Add apply button
button_ax = plt.axes([0.01, 0.02, 0.10, 0.04])
apply_button = Button(button_ax, 'Apply Curve')

def parse_and_plot_equation(equation_str):
    """Parse equation string and plot it."""
    import re
    try:
        # Clean up equation
        eq = equation_str.strip()
        if eq.startswith("y =") or eq.startswith("y="):
            eq = eq.split("=", 1)[1].strip()

        # Replace common notations
        eq = eq.replace("^", "**")

        # Add * for implicit multiplication: number followed by x (but not in exponent like e-05)
        # Handle: "2x" -> "2*x", "2.5x" -> "2.5*x", "-3x" -> "-3*x"
        # But NOT "e-05x" which should stay as coefficient
        eq = re.sub(r'(\d)([x])', r'\1*\2', eq)  # 2x -> 2*x
        eq = re.sub(r'(\d)\s+([x])', r'\1*\2', eq)  # 2 x -> 2*x
        eq = re.sub(r'([x])\s*\*\*\s*(\d)', r'\1**\2', eq)  # x ** 2 -> x**2

        # Remove remaining spaces
        eq = eq.replace(" ", "")

        print(f"Parsing: {eq}")

        # Evaluate for all x values
        y_custom = np.full_like(x, np.nan, dtype=float)
        x_vals = x[nonzero_mask]
        y_vals = []
        for xi in x_vals:
            result = eval(eq, {"__builtins__": {}, "x": xi, "np": np,
                              "sin": np.sin, "cos": np.cos, "exp": np.exp,
                              "sqrt": np.sqrt, "pi": np.pi, "abs": abs})
            y_vals.append(result)
        y_custom[nonzero_mask] = y_vals

        # Update custom line
        custom_line.set_data(x, y_custom)
        custom_line.set_visible(True)

        # Calculate R² against raw speed data
        if SPEED_COLUMN in df.columns:
            raw = df[SPEED_COLUMN].to_numpy(dtype=float)
            y_raw = raw[nonzero_mask]
            y_pred = np.array(y_vals)
            ss_res = np.sum((y_raw - y_pred) ** 2)
            ss_tot = np.sum((y_raw - np.mean(y_raw)) ** 2)
            r2 = 1 - ss_res / ss_tot if ss_tot > 0 else 0
            custom_line.set_label(f"Custom curve (R²={r2:.4f})")
            print(f"Custom curve R² vs raw data: {r2:.6f}")

        ax.legend(loc="upper right", fontsize=7)
        fig.canvas.draw_idle()
        print(f"Successfully plotted custom curve")

    except Exception as e:
        print(f"Error parsing equation: {e}")
        print("Use format like: 0.001*x^2 + 0.5*x + 60")
        print("Or with explicit multiply: 1.5e-05*x^2 - 0.02*x + 60")

def on_apply(event):
    parse_and_plot_equation(textbox.text)

def on_submit(text):
    parse_and_plot_equation(text)

apply_button.on_clicked(on_apply)
textbox.on_submit(on_submit)

# Print equations in console for reference
print("\n=== Polynomial Equations (copy-paste ready) ===")
for label, info in poly_fits.items():
    print(f"{label}: {info['equation']}")

print(f"\n=== Data x-range (non-zero): {x_min_nz:.0f} to {x_max_nz:.0f} ===")
print("Enter custom equation in the text box (e.g., '0.001x^2 + 0.5x + 60')")
print("Supports: x, ^, sin, cos, exp, sqrt, pi")

plt.show()
