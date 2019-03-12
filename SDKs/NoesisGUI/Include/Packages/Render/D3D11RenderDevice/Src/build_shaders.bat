@echo off

del Shaders.h

:: Vertex Shaders

CALL :fxc ShaderVS.hlsl /T vs_4_0 /Vn Pos_VS /DHAS_POSITION
CALL :fxc ShaderVS.hlsl /T vs_4_0 /Vn PosColor_VS /DHAS_POSITION /DHAS_COLOR
CALL :fxc ShaderVS.hlsl /T vs_4_0 /Vn PosTex0_VS /DHAS_POSITION /DHAS_UV0
CALL :fxc ShaderVS.hlsl /T vs_4_0 /Vn PosColorCoverage_VS /DHAS_POSITION /DHAS_COLOR /DHAS_COVERAGE
CALL :fxc ShaderVS.hlsl /T vs_4_0 /Vn PosTex0Coverage_VS /DHAS_POSITION /DHAS_UV0 /DHAS_COVERAGE
CALL :fxc ShaderVS.hlsl /T vs_4_0 /Vn PosColorTex1_VS /DHAS_POSITION /DHAS_COLOR /DHAS_UV1
CALL :fxc ShaderVS.hlsl /T vs_4_0 /Vn PosTex0Tex1_VS /DHAS_POSITION /DHAS_UV0 /DHAS_UV1

:: Pixel Shaders

CALL :fxc ShaderPS.hlsl /T ps_4_0 /Vn RGBA_FS /DEFFECT_RGBA
CALL :fxc ShaderPS.hlsl /T ps_4_0 /Vn Mask_FS /DEFFECT_MASK
CALL :fxc ShaderPS.hlsl /T ps_4_0 /Vn PathSolid_FS /DEFFECT_PATH_SOLID
CALL :fxc ShaderPS.hlsl /T ps_4_0 /Vn PathLinear_FS /DEFFECT_PATH_LINEAR
CALL :fxc ShaderPS.hlsl /T ps_4_0 /Vn PathRadial_FS /DEFFECT_PATH_RADIAL
CALL :fxc ShaderPS.hlsl /T ps_4_0 /Vn PathPattern_FS /DEFFECT_PATH_PATTERN
CALL :fxc ShaderPS.hlsl /T ps_4_0 /Vn PathAASolid_FS /DEFFECT_PATH_AA_SOLID
CALL :fxc ShaderPS.hlsl /T ps_4_0 /Vn PathAALinear_FS /DEFFECT_PATH_AA_LINEAR
CALL :fxc ShaderPS.hlsl /T ps_4_0 /Vn PathAARadial_FS /DEFFECT_PATH_AA_RADIAL
CALL :fxc ShaderPS.hlsl /T ps_4_0 /Vn PathAAPattern_FS /DEFFECT_PATH_AA_PATTERN
CALL :fxc ShaderPS.hlsl /T ps_4_0 /Vn ImagePaintOpacitySolid_FS /DEFFECT_IMAGE_PAINT_OPACITY_SOLID
CALL :fxc ShaderPS.hlsl /T ps_4_0 /Vn ImagePaintOpacityLinear_FS /DEFFECT_IMAGE_PAINT_OPACITY_LINEAR
CALL :fxc ShaderPS.hlsl /T ps_4_0 /Vn ImagePaintOpacityRadial_FS /DEFFECT_IMAGE_PAINT_OPACITY_RADIAL
CALL :fxc ShaderPS.hlsl /T ps_4_0 /Vn ImagePaintOpacityPattern_FS /DEFFECT_IMAGE_PAINT_OPACITY_PATTERN
CALL :fxc ShaderPS.hlsl /T ps_4_0 /Vn TextSolid_FS /DEFFECT_TEXT_SOLID
CALL :fxc ShaderPS.hlsl /T ps_4_0 /Vn TextLinear_FS /DEFFECT_TEXT_LINEAR
CALL :fxc ShaderPS.hlsl /T ps_4_0 /Vn TextRadial_FS /DEFFECT_TEXT_RADIAL
CALL :fxc ShaderPS.hlsl /T ps_4_0 /Vn TextPattern_FS /DEFFECT_TEXT_PATTERN

:: Clear and Resolve 

CALL :fxc QuadVS.hlsl /T vs_4_0 /Vn Quad_VS
CALL :fxc ClearPS.hlsl /T ps_4_0 /Vn Clear_PS
CALL :fxc ResolvePS.hlsl /T ps_4_0 /Vn Resolve2_PS /D NUM_SAMPLES=2
CALL :fxc ResolvePS.hlsl /T ps_4_0 /Vn Resolve4_PS /D NUM_SAMPLES=4
CALL :fxc ResolvePS.hlsl /T ps_4_0 /Vn Resolve8_PS /D NUM_SAMPLES=8
CALL :fxc ResolvePS.hlsl /T ps_4_0 /Vn Resolve16_PS /D NUM_SAMPLES=16

del out.tmp

EXIT /B 0

:: -------------------------------------------------------------------------------------------------------
:fxc
    fxc /nologo /O3 /Qstrip_reflect /Fh out.tmp %* > NUL
    type out.tmp >> Shaders.h
    EXIT /B 0