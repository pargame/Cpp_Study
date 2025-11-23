# PowerShell Script: 모든 .cpp 파일에 표준 헤더 추가
# 사용법: .\add_headers_to_cpp.ps1

$weekFolders = 1..32 | ForEach-Object { "Week$_" }

foreach ($week in $weekFolders) {
    $srcPath = Join-Path $PSScriptRoot "$week\src"
    
    if (-not (Test-Path $srcPath)) {
        Write-Host "⏭️  $week\src not found, skipping..." -ForegroundColor Yellow
        continue
    }
    
    $cppFiles = Get-ChildItem -Path $srcPath -Filter "*.cpp"
    
    foreach ($file in $cppFiles) {
        Write-Host "📝 Processing $week\src\$($file.Name)" -ForegroundColor Cyan
        
        $content = Get-Content $file.FullName -Raw
        
        # 이미 헤더가 있는지 확인 (Week 키워드로 판단)
        if ($content -match "^//\s*Week\d+") {
            Write-Host "   ✅ Header already exists, skipping" -ForegroundColor Green
            continue
        }
        
        # 파일명에서 번호와 이름 추출 (예: 01_example.cpp -> 01, example)
        if ($file.Name -match "^(\d+)_(.+)\.cpp$") {
            $fileNum = $Matches[1]
            $fileName = $Matches[2].Replace('_', ' ')
            
            # 기본 헤더 템플릿 생성
            $header = @"
// $week - $($file.Name)
// $fileName 예제
//
// 핵심 개념:
// - (TODO: 핵심 개념 추가 필요)
//
// 예상 출력:
// (TODO: 실행 결과 주석 추가 필요)

"@
            
            # 기존 내용 앞에 헤더 추가
            $newContent = $header + $content
            
            # 파일 저장
            Set-Content -Path $file.FullName -Value $newContent -Encoding UTF8
            Write-Host "   ✅ Header added" -ForegroundColor Green
        }
    }
}

Write-Host "`n🎉 All done! Please review files and fill in TODO sections." -ForegroundColor Magenta
