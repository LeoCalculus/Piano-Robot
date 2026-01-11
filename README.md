# ELEC-391  
ELEC 391  

# Requirements for using debug:  
1. Download openocd for windows at: https://github.com/openocd-org/openocd/releases/tag/v0.12.0
2. Save environment variable path for openocd, create a new variable in either User or System environment variable called "OPENOCD".
3. Relaunch VSCode and debug mode is ready

# Tips for debug  
Do not use step into if it is a library function, use step over instead. 
Only use step into if the function written by yourself. Otherwise, you may have trouble.  