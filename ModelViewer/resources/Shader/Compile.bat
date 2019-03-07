fxc.exe /T vs_5_0 /E vs_direction /Fh directionVS.h Direction/vertex.vsh
fxc.exe /T ps_5_0 /E ps_direction /Fh directionPS.h Direction/pixel.psh
fxc.exe /T vs_5_0 /E vs_point /Fh pointVS.h Point/vertex.vsh
fxc.exe /T ps_5_0 /E ps_point /Fh pointPS.h Point/pixel.psh
fxc.exe /T vs_5_0 /E vs_texture /Fh textureVS.h Texture/vertex.vsh
fxc.exe /T ps_5_0 /E ps_texture /Fh texturePS.h Texture/pixel.psh
fxc.exe /T vs_5_0 /E vs_wireframe /Fh wireframeVS.h WireFrame/vertex.vsh
fxc.exe /T ps_5_0 /E ps_wireframe /Fh wireframePS.h WireFrame/pixel.psh
fxc.exe /T vs_5_0 /E vs_skycolor /Fh skycolorVS.h SkyColor/vertex.vsh
fxc.exe /T ps_5_0 /E ps_skycolor /Fh skycolorPS.h SkyColor/pixel.psh
timeout /t 100 /nobreak >nul