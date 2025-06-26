# Obtiene el hash y la fecha del último commit
$hash = git log -1 --format="%h"
$date = (git show -s --format=%cd --date=format:"%y-%m-%d-%H-%M" $hash).Trim()
$baseName = "$date-$hash"

# Directorios de destino
$repoDiffDir = ".\(D)Proceso\Historial de cambios\Diffs\RepoDiffs"
$codeDiffDir = ".\(D)Proceso\Historial de cambios\Diffs\CodeDiffs"

if (-not (Test-Path $repoDiffDir)) {
    New-Item -ItemType Directory -Path $repoDiffDir -Force | Out-Null
}
if (-not (Test-Path $codeDiffDir)) {
    New-Item -ItemType Directory -Path $codeDiffDir -Force | Out-Null
}

# 1) Cambios staged fuera de codigo_fuente y excluyendo Historial de cambios/Diffs
$outsideFile = Join-Path $repoDiffDir "cambiosRepo-stage(current).diff"
git diff --cached -- . `
    ':(exclude)codigo_fuente/**' `
    ':(exclude)(D)Proceso/Historial de cambios/**' `
    > $outsideFile
if ((Get-Content $outsideFile).Length -eq 0) {
    "No se modificaron elementos fuera de codigo_fuente/ y (D)Proceso/Historial de cambios/" | Set-Content $outsideFile
}
Write-Host "Generado: $outsideFile"

# 2) Cambios staged solo en la carpeta codigo_fuente
$insideFile = Join-Path $codeDiffDir "cambios-stage(current).diff"
git diff --cached -- codigo_fuente/ ':(exclude)**/*.md' > $insideFile
if ((Get-Content $insideFile).Length -eq 0) {
    "No se modificaron archivos del codigo_fuente del proyecto" | Set-Content $insideFile
}
Write-Host "Generado: $insideFile"

# Archivo para guardar el hash del último commit para el que se generaron los diffs
$lastCommitFile = ".last-diff-commit"

# Leer el hash anterior (si existe)
$lastCommitHash = ""
if (Test-Path $lastCommitFile) {
    $lastCommitHash = Get-Content $lastCommitFile
}

# Solo renombrar si el commit actual es diferente al último para el que se generaron los diffs

if ($hash -ne $lastCommitHash) {
    $oldRepoDiff = Join-Path $repoDiffDir "cambiosRepo-stage(current).diff"
    $newRepoDiff = Join-Path $repoDiffDir "cambiosRepo-$baseName.diff"
    if (Test-Path $oldRepoDiff -and ($oldRepoDiff -ne $newRepoDiff)) {
        if (Test-Path $newRepoDiff) { Remove-Item $newRepoDiff -Force }
        Rename-Item $oldRepoDiff $newRepoDiff -Force
        Write-Host "Renombrado: $oldRepoDiff -> $newRepoDiff"
    }

    $oldCodeDiff = Join-Path $codeDiffDir "cambios-stage(current).diff"
    $newCodeDiff = Join-Path $codeDiffDir "cambios-$baseName.diff"
    if (Test-Path $oldCodeDiff -and ($oldCodeDiff -ne $newCodeDiff)) {
        if (Test-Path $newCodeDiff) { Remove-Item $newCodeDiff -Force }
        Rename-Item $oldCodeDiff $newCodeDiff -Force
        Write-Host "Renombrado: $oldCodeDiff -> $newCodeDiff"
    }

    Set-Content $lastCommitFile $hash
}