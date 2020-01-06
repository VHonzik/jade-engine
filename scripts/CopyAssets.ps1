param (
	[Parameter()][string]$AssetDirectory = "assets",
	[Parameter(Mandatory)][string]$OutputFolder
)

Write-Output "Copying assets from $AssetDirectory to $OutputFolder ..."

xcopy $AssetDirectory\*.ttf $OutputFolder\assets /C /D /Y /K /I
xcopy $AssetDirectory\*.png $OutputFolder\assets /C /D /Y /K /I
xcopy $AssetDirectory\*.wav $OutputFolder\assets /C /D /Y /K /I
xcopy $AssetDirectory\*.json $OutputFolder\assets /C /D /Y /K /I

