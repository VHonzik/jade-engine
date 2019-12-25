param (
	[Parameter(Mandatory)][string]$JadeEngineDirectory,
	[Parameter(Mandatory)][string]$OutputFolder
)

#Write-Debug "Copying dependencies from $JadeEngineDirectory to $OutputFolder ..."

#Write-Debug "Copying dlls..."
xcopy $JadeEngineDirectory\thirdparty\SDL2-2.0.8\lib\x64\*.dll $OutputFolder /C /D /Y /K /I
xcopy $JadeEngineDirectory\thirdparty\SDL2_ttf-2.0.14\lib\x64\*.dll $OutputFolder /C /D /Y /K /I
xcopy $JadeEngineDirectory\thirdparty\SDL2_image-2.0.3\lib\x64\*.dll $OutputFolder /C /D /Y /K /I
xcopy $JadeEngineDirectory\thirdparty\SDL2_mixer-2.0.4\lib\x64\*.dll $OutputFolder /C /D /Y /K /I
xcopy $JadeEngineDirectory\thirdparty\SDL2_image-2.0.3\lib\x64\zlib1.dll $OutputFolder /C /Y /K /I

#Write-Debug "Copying dlls..."
xcopy $JadeEngineDirectory\assets\*.ttf $OutputFolder\assets /C /D /Y /K /I
xcopy $JadeEngineDirectory\assets\*.png $OutputFolder\assets /C /D /Y /K /I
xcopy $JadeEngineDirectory\assets\*.wav $OutputFolder\assets /C /D /Y /K /I
xcopy $JadeEngineDirectory\assets\*.json $OutputFolder\assets /C /D /Y /K /I

