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
    $status += $readmeOk ? "README OK; " : "README MISSING; "
    $status += $srcOk ? "SRC OK ($cppCount cpp files); " : "SRC MISSING; "
    $status += $cmakeOk ? "CMake OK; " : "CMake MISSING; "
    $status += $buildOk ? "BuildScript OK" : "BuildScript MISSING"
    Write-Output $status
}
