$weeks = 1..32
foreach ($w in $weeks) {
    $weekDir = "e:/repos/C++_Study/Week$w"
    $readme = Join-Path $weekDir "README.md"
    $srcDir = Join-Path $weekDir "src"
    $srcExists = Test-Path $srcDir
    $cppFiles = if ($srcExists) { Get-ChildItem -Path $srcDir -Filter *.cpp -File | Measure-Object }.Count else { 0 }
    $status = "[Week$w] "
    if (Test-Path $readme) { $status += "README OK; " } else { $status += "README MISSING; " }
    if ($srcExists -and $cppFiles -gt 0) { $status += "SRC OK ($cppFiles cpp files)" } else { $status += "SRC MISSING or empty" }
    Write-Output $status
}
