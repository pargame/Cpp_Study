$weeks = 1..32
foreach ($w in $weeks) {
    $weekDir = "e:/repos/C++_Study/Week$w"
    $readme = Join-Path $weekDir "README.md"
    $srcDir = Join-Path $weekDir "src"
    $cmake = Join-Path $weekDir "CMakeLists.txt"
    $buildScript = Join-Path $weekDir "build_cmake.bat"
    $readmeOk = Test-Path $readme
    $srcOk = Test-Path $srcDir
    $cppCount = if ($srcOk) { (Get-ChildItem -Path $srcDir -Filter *.cpp -File | Measure-Object).Count } else { 0 }
    $cmakeOk = Test-Path $cmake
    $buildOk = Test-Path $buildScript
    $status = "[Week$w] "
    if ($readmeOk) { $status += "README OK; " } else { $status += "README MISSING; " }
    if ($srcOk) { $status += "SRC OK ($cppCount cpp files); " } else { $status += "SRC MISSING; " }
    if ($cmakeOk) { $status += "CMake OK; " } else { $status += "CMake MISSING; " }
    if ($buildOk) { $status += "BuildScript OK" } else { $status += "BuildScript MISSING" }
    Write-Output $status
}
