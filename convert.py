import re

# 1. 定义你的宏 (Macros) - 单位：mm
# 如果你在 C 代码里改了这些值，记得在这里同步修改
MACROS = {
    "CHORD_1": 3 * 21.0,
    "CHORD_2": 4 * 21.0,
    "CHORD_3": 3 * 21.0,
    "CHORD_4": 3 * 21.0,
    "DO_3":    15 * 21.0,
    "REb_3":   16 * 21.0,
    "SIb_2":   18 * 21.0,
    "LAb_2":   19 * 21.0,
    "MIb_2":   22 * 21.0,
    "FA_2":    19 * 21.0,
    "SO_2":    18 * 21.0,
    "FA_3":    12 * 21.0,
    "MIb_3":   15 * 21.0,
    "MIb_4":   8 * 21.0,
    "LA_4":    4 * 21.0,
    "LA_3":    11 * 21.0,
    "FA_4":    5 * 21.0,
    "SIb_3":   11* 21.0,
    "DO_4":    8 * 21.0,
    "SO_3":    11 * 21.0,
    "LAb_3":   12 * 21.0,
    "REb_4":   9 * 21.0,
    "SO_4":    4 * 21.0,
    "0.0f":    0.0,
    "1000.0f": 1000.0
}

def parse_c_chord_data(c_code):
    """解析 C 语言 debug_data 数组并输出为 txt 格式"""
    # 正则表达式匹配结构体内容
    pattern = re.compile(
        r"\{\s*\.positions\s*=\s*\{(?P<lpos>[^,]+),\s*(?P<rpos>[^}]+)\},\s*"
        r"\.pressed\s*=\s*\{(?P<pressed>[^}]+)\},\s*"
        r"\.duration_ms\s*=\s*(?P<dur>\d+),\s*"
        r"\.long_pressed\s*=\s*\{(?P<long>[^}]+)\}\s*\}"
    )

    output_lines = []

    for match in pattern.finditer(c_code):
        # 处理位置逻辑
        lpos_raw = match.group("lpos").strip()
        rpos_raw = match.group("rpos").strip()

        # 核心修复逻辑：先查宏定义，再统一转成字符串处理 'f' 后缀
        def process_val(val_str):
            val = MACROS.get(val_str, val_str)
            if isinstance(val, (int, float)):
                return str(val)
            return val.replace('f', '').strip()

        lpos = process_val(lpos_raw)
        rpos = process_val(rpos_raw)

        # 处理按键状态
        pressed = match.group("pressed").replace(" ", "").split(",")
        dur = match.group("dur")

        # 处理长按标志
        long_press = match.group("long").replace(" ", "").split(",")
        keep_l = long_press[0].strip()
        keep_r = long_press[1].strip()

        # 组装成一行
        line = f"{lpos} {rpos} {' '.join(pressed)} {dur} {keep_l} {keep_r}"
        output_lines.append(line)

    return "\n".join(output_lines)

# --- 使用示例 ---
if __name__ == "__main__":
    # 将你那一大串 C 代码贴在下面这个三引号字符串里
    raw_c_data = """
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 0
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 1
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 2
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 3
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 4
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 5
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 7
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 8
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 9
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 10
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 11
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 12
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 13
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 14
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 16
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 17
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 18
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 19
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 20
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 23
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 24
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 25
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 26
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 27
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 28
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 29
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 30
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        // ================== 2 (32-63) ==================
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 0
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 1
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 2
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 3
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 4
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 5
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 7
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 8
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 9
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 10
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 11
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 12
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 13
        { .positions = {CHORD_2, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 14
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 16
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 17
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 18
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 19
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 20
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 23
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 24

        // start 
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 25
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 26
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 27
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 28
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 29
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 30
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        // ================== 3 (64-95) ==================
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 0
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 1
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 2
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 3
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 4
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 5
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 7
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 8
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 9
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 10
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 11
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 12
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 13
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 14
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, SIb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 16
        { .positions = {CHORD_3, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 17
        { .positions = {CHORD_3, SIb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 18
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 19
        { .positions = {CHORD_3, SIb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 20
        { .positions = {0.0f , MIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {0.0f , MIb_2}, .pressed = {0, 0, 0, 0, 0, 1, 0, 1, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        // line 8
        { .positions = {CHORD_4, FA_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 23
        { .positions = {CHORD_4, FA_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 24
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 25
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 26
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 27
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 28
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 29
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 30
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        // ================== 4 (96-127) ==================
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 0
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 1
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 2
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 3
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 4
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 5
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 7
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 8
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 9
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 10
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 11
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 12
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 13
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 14
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, SIb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 16
        { .positions = {CHORD_3, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 17
        { .positions = {CHORD_3, SIb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 18
        { .positions = {CHORD_3, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 19
        { .positions = {CHORD_3, SIb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 20
        { .positions = {CHORD_3, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, DO_3}, .pressed =  {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        { .positions = {CHORD_4, LAb_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 23
        { .positions = {CHORD_4, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 24
        { .positions = {CHORD_4, FA_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 25
        { .positions = {CHORD_4, FA_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 26
        { .positions = {CHORD_4, FA_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 27
        { .positions = {CHORD_4, FA_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 28
        { .positions = {CHORD_4, FA_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 29
        { .positions = {CHORD_4, FA_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 30
        { .positions = {CHORD_4, FA_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        // ================== 5 (128-159) ==================
        { .positions = {CHORD_1, LAb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 0
        { .positions = {CHORD_1, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 1
        { .positions = {CHORD_1, LAb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 2
        { .positions = {CHORD_1, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 3
        { .positions = {CHORD_1, LAb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 4
        { .positions = {CHORD_1, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 5
        { .positions = {CHORD_1, LAb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, SO_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 7
        { .positions = {CHORD_2, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 8
        { .positions = {CHORD_2, SO_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 9
        { .positions = {CHORD_2, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 10
        { .positions = {CHORD_2, SO_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 11
        { .positions = {CHORD_2, SO_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 12
        { .positions = {CHORD_2, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 13
        { .positions = {CHORD_2, SO_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 14
        { .positions = {CHORD_3, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 16
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 17
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 18
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 19
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 20
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        { .positions = {CHORD_4, SO_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 23
        { .positions = {CHORD_4, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 24
        { .positions = {CHORD_4, SO_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 25
        { .positions = {CHORD_4, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 26
        { .positions = {CHORD_4, SO_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 27
        { .positions = {CHORD_4, SO_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 28
        { .positions = {CHORD_4, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 29
        { .positions = {CHORD_4, SO_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 30
        { .positions = {CHORD_4, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        // ================== 6 (160-191) ==================
        { .positions = {CHORD_1, LAb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 0
        { .positions = {CHORD_1, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 1
        { .positions = {CHORD_1, LAb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 2
        { .positions = {CHORD_1, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 3
        { .positions = {CHORD_1, LAb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 4
        { .positions = {CHORD_1, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 5
        { .positions = {CHORD_1, LAb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, SO_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 7
        { .positions = {CHORD_2, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 8
        { .positions = {CHORD_2, SO_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 9
        { .positions = {CHORD_2, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 10
        { .positions = {CHORD_2, SO_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 11
        { .positions = {CHORD_2, SO_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 12
        { .positions = {CHORD_2, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 13
        { .positions = {CHORD_2, SO_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 14
        { .positions = {CHORD_3, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 16
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 17
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 18
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 19
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 20
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        { .positions = {CHORD_4, SO_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 23
        { .positions = {CHORD_4, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 24

        // start
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 25
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 26
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 27
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 28
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 29
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 30
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        // ================== 7 (192-223) ==================
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 0
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 1
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 2
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 3
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 4
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 5
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 7
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 8
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 9
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 10
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 11
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 12
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 13
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 14
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, SIb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 16
        { .positions = {CHORD_3, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 17
        { .positions = {CHORD_3, SIb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 18
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 19
        { .positions = {CHORD_3, SIb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 20
        { .positions = {CHORD_3, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        { .positions = {CHORD_4, LAb_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 23
        { .positions = {CHORD_4, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 24
        { .positions = {CHORD_4, SO_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 25
        { .positions = {CHORD_4, FA_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 26
        { .positions = {CHORD_4, FA_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 27
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 28
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 29
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 30
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        // ================== 7 (224-255) ==================
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 0
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 1
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 2
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 3
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 4
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 5
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 7
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 8
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 9
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 10
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 11
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 12
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 13
        { .positions = {CHORD_2, LAb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 14
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 16
        { .positions = {CHORD_3, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 17
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 18
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 19
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 20
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        // line 28
        { .positions = {CHORD_4, SIb_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 23
        { .positions = {CHORD_4, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 24
        { .positions = {CHORD_4, LAb_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 25
        { .positions = {CHORD_4, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 26
        { .positions = {CHORD_4, FA_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 27
        { .positions = {CHORD_4, FA_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 28
        { .positions = {CHORD_4, FA_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 29
        { .positions = {CHORD_4, FA_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 30
        { .positions = {CHORD_4, FA_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        // ================== 8 (256-287) ==================
        { .positions = {CHORD_1, FA_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 0
        { .positions = {CHORD_1, FA_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 1
        { .positions = {CHORD_1, FA_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 2
        { .positions = {CHORD_1, FA_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 3
        { .positions = {CHORD_1, FA_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 4
        { .positions = {CHORD_1, FA_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 5
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, MIb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 7
        { .positions = {CHORD_2, MIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 8
        { .positions = {CHORD_2, MIb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 9
        { .positions = {CHORD_2, MIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 10
        { .positions = {CHORD_2, MIb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 11
        { .positions = {CHORD_2, MIb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 12
        { .positions = {CHORD_2, MIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 13
        { .positions = {CHORD_2, MIb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 14
        { .positions = {CHORD_3, MIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, MIb_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 16
        { .positions = {CHORD_3, MIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 17
        { .positions = {CHORD_3, MIb_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 18
        { .positions = {CHORD_3, MIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 19
        { .positions = {CHORD_3, MIb_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 20
        { .positions = {CHORD_3, MIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, REb_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 22

        { .positions = {CHORD_4, REb_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 23
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 24
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 25
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 26
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 27
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 28
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 29
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 30
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        // line 36

        // ================== 9 (288-319) ==================
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 0
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 1
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 2
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 3
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 4
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 5
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 7
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 8
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 9
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 10
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 11
        { .positions = {CHORD_2, LAb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 12
        { .positions = {CHORD_2, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 13
        { .positions = {CHORD_2, LAb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 14
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 16
        { .positions = {CHORD_3, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 17
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 18
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 19
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 20
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        { .positions = {CHORD_4, SIb_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 23
        { .positions = {CHORD_4, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 24
        { .positions = {CHORD_4, LAb_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 25
        { .positions = {CHORD_4, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 26
        { .positions = {CHORD_4, LAb_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 27
        { .positions = {CHORD_4, MIb_4}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 28
        { .positions = {CHORD_4, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 29
        { .positions = {CHORD_4, MIb_4}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 30
        { .positions = {CHORD_4, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        // line 40
        // ================== 10 (320-351) ==================
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 0
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 1
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 2
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 3
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 4
        { .positions = {CHORD_1, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 5
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, MIb_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 7
        { .positions = {CHORD_2, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 8
        { .positions = {CHORD_2, MIb_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 9
        { .positions = {CHORD_2, SIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 10
        { .positions = {CHORD_2, DO_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 11
        { .positions = {CHORD_2, DO_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 12
        { .positions = {CHORD_2, REb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 13
        { .positions = {CHORD_2, REb_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 14
        { .positions = {CHORD_3, REb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 16
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 17
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 18
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 19
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 20
        { .positions = {CHORD_3, DO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        { .positions = {CHORD_4, DO_4}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 23
        { .positions = {CHORD_4, DO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 24
        { .positions = {CHORD_4, DO_4}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 25
        { .positions = {CHORD_4, FA_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 26
        { .positions = {CHORD_4, SO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 27
        { .positions = {CHORD_4, SO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 28
        { .positions = {CHORD_4, LAb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 29
        { .positions = {CHORD_4, LAb_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 30
        { .positions = {CHORD_4, LAb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        // line 44
        // ================== 11 (352-383) ==================
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 0
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 1
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 2
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 3
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 4
        { .positions = {CHORD_1, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 5
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, MIb_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 7
        { .positions = {CHORD_2, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 8
        { .positions = {CHORD_2, MIb_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 9
        { .positions = {CHORD_2, SIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 10
        { .positions = {CHORD_2, DO_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 11
        { .positions = {CHORD_2, DO_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 12
        { .positions = {CHORD_2, REb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 13
        { .positions = {CHORD_2, REb_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 14
        { .positions = {CHORD_3, DO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 16
        { .positions = {CHORD_3, DO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 17
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 18
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 19
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 20
        { .positions = {CHORD_3, DO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        { .positions = {CHORD_4, DO_4}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 23
        { .positions = {CHORD_4, DO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 24
        { .positions = {CHORD_4, FA_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 25
        { .positions = {CHORD_4, FA_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 26
        { .positions = {CHORD_4, SO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 27
        { .positions = {CHORD_4, SO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 28
        { .positions = {CHORD_4, LAb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 29
        { .positions = {CHORD_4, LAb_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 30
        { .positions = {CHORD_4, LAb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        // line 48
        // ================== 12 (384-415) ==================
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 0
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 1
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 2
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 3
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 4
        { .positions = {CHORD_1, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 5
        { .positions = {CHORD_1, FA_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, SO_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 7
        { .positions = {CHORD_2, SO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 8
        { .positions = {CHORD_2, SO_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 9
        { .positions = {CHORD_2, SO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 10
        { .positions = {CHORD_2, SO_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 11
        { .positions = {CHORD_2, SO_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 12
        { .positions = {CHORD_2, SO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 13
        { .positions = {CHORD_2, SO_4}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 14
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 16
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 17
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 18
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 19
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 20
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, MIb_4}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        { .positions = {CHORD_4, DO_4}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 23
        { .positions = {CHORD_4, REb_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 24
        { .positions = {CHORD_4, DO_4}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 25
        { .positions = {CHORD_4, DO_4}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 26
        { .positions = {CHORD_4, DO_4}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 27
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 28
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 29
        { .positions = {CHORD_4, DO_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 30
        { .positions = {CHORD_4, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 31

        // line 52
        // ================== 13 (416-447) ==================
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 0
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 1
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 2
        { .positions = {CHORD_1, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 3
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 4
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 5
        { .positions = {CHORD_1, REb_3}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 7
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 8
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 9
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 10
        { .positions = {CHORD_2, SIb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 11
        { .positions = {CHORD_2, LAb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 12
        { .positions = {CHORD_2, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 13
        { .positions = {CHORD_2, SO_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 14
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, SO_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 16
        { .positions = {CHORD_3, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 17
        { .positions = {CHORD_3, SO_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 18
        { .positions = {CHORD_3, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 19
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 20
        { .positions = {CHORD_3, DO_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, SO_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        { .positions = {CHORD_4, LAb_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 1} }, // 23
        { .positions = {CHORD_4, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 24
        { .positions = {CHORD_4, LAb_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 25
        { .positions = {CHORD_4, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 26
        { .positions = {CHORD_4, LAb_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 27
        { .positions = {CHORD_4, FA_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 28
        { .positions = {CHORD_4, MIb_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 29
        { .positions = {CHORD_4, FA_3}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 30
        { .positions = {CHORD_4, LA_3}, .pressed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        // ================== 5 (128-159) ==================
        { .positions = {CHORD_1, LAb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 0
        { .positions = {CHORD_1, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 1
        { .positions = {CHORD_1, LAb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 2
        { .positions = {CHORD_1, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 3
        { .positions = {CHORD_1, LAb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 4
        { .positions = {CHORD_1, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 5
        { .positions = {CHORD_1, LAb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, SO_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 7
        { .positions = {CHORD_2, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 8
        { .positions = {CHORD_2, SO_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 9
        { .positions = {CHORD_2, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 10
        { .positions = {CHORD_2, SO_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 11
        { .positions = {CHORD_2, SO_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 12
        { .positions = {CHORD_2, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 13
        { .positions = {CHORD_2, SO_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 14
        { .positions = {CHORD_3, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 16
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 17
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 18
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 19
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 20
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22

        { .positions = {CHORD_4, SO_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 23
        { .positions = {CHORD_4, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 24
        { .positions = {CHORD_4, SO_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 25
        { .positions = {CHORD_4, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 26
        { .positions = {CHORD_4, SO_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 27
        { .positions = {CHORD_4, SO_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 28
        { .positions = {CHORD_4, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 29
        { .positions = {CHORD_4, SO_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 30
        { .positions = {CHORD_4, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 31

        // ================== 6 (160-191) ==================
        { .positions = {CHORD_1, LAb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 0
        { .positions = {CHORD_1, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 1
        { .positions = {CHORD_1, LAb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 2
        { .positions = {CHORD_1, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 3
        { .positions = {CHORD_1, LAb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 4
        { .positions = {CHORD_1, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 5
        { .positions = {CHORD_1, LAb_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 6

        { .positions = {CHORD_2, SO_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 7
        { .positions = {CHORD_2, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 8
        { .positions = {CHORD_2, SO_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 9
        { .positions = {CHORD_2, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 10
        { .positions = {CHORD_2, SO_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 11
        { .positions = {CHORD_2, SO_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 12
        { .positions = {CHORD_2, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 13
        { .positions = {CHORD_2, SO_2}, .pressed = {0, 1, 0, 0, 0, 1, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 14
        { .positions = {CHORD_3, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 15

        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 16
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 17
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 18
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 19
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 20
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 21
        { .positions = {CHORD_3, LAb_2}, .pressed = {0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 22
        { .positions = {CHORD_4, SO_2}, .pressed = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 23
        { .positions = {CHORD_4, SO_2}, .pressed = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .duration_ms = 250, .long_pressed = {0, 0} }, // 24
    """

    result = parse_c_chord_data(raw_c_data)
    
    # 保存到文件
    with open("debug_song.txt", "w") as f:
        f.write(result)
    
    print("转换完成！已生成 debug_song.txt")