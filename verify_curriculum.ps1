$root = "e:/repos/C++_Study"
$missing_count = 0

Write-Host "=== Curriculum Verification Start ===" -ForegroundColor Cyan

1..32 | ForEach-Object {
    $week = "Week$_"
    $path = Join-Path $root $week
    
    if (-not (Test-Path $path)) {
        Write-Host "[$week] MISSING DIRECTORY" -ForegroundColor Red
        $missing_count++
        return
    }

    $required_files = @("README.md", "CMakeLists.txt", "build_cmake.bat")
    $week_missing = @()

    foreach ($file in $required_files) {
        if (-not (Test-Path (Join-Path $path $file))) {
            $week_missing += $file
        }
    }

    if (-not (Test-Path (Join-Path $path "src"))) {
        $week_missing += "src/"
    }

    if ($week_missing.Count -gt 0) {
        Write-Host "[$week] INCOMPLETE. Missing: $($week_missing -join ', ')" -ForegroundColor Yellow
        $missing_count++
    } else {
        Write-Host "[$week] OK" -ForegroundColor Green
    }
}

Write-Host "`n=== Verification Complete ===" -ForegroundColor Cyan
if ($missing_count -eq 0) {
    Write-Host "All weeks are structurally complete!" -ForegroundColor Green
} else {
    Write-Host "Found issues in $missing_count weeks." -ForegroundColor Red
}
