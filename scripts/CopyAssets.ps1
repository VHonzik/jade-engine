param (
	[Parameter()][string]$AssetDirectory = "assets",
	[Parameter(Mandatory)][string]$OutputFolder
)

$IsAssetPathAbsolute = [System.IO.Path]::IsPathRooted($AssetDirectory)
$AssetAbsoluteDirectory = $AssetDirectory
if (-not $IsAssetPathAbsolute)
{
	$AssetAbsoluteDirectory = Resolve-Path $AssetDirectory
}

Write-Output "Copying assets from $AssetAbsoluteDirectory to $OutputFolder\assets ..."

xcopy $AssetAbsoluteDirectory\*.ttf $OutputFolder\assets /C /D /Y /K /I
xcopy $AssetAbsoluteDirectory\*.png $OutputFolder\assets /C /D /Y /K /I
xcopy $AssetAbsoluteDirectory\*.wav $OutputFolder\assets /C /D /Y /K /I
xcopy $AssetAbsoluteDirectory\*.json $OutputFolder\assets /C /D /Y /K /I

