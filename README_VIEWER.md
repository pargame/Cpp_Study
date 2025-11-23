# C++ Study 주차별 README 뷰어 웹앱

## 📖 소개

`viewer` 디렉토리는 Week1~Week32까지의 모든 README.md 파일을 브라우저에서 쉽게 탐색할 수 있는 React 웹 애플리케이션입니다.

## 🚀 빠른 시작

```powershell
# viewer 디렉토리로 이동
cd viewer

# 개발 서버 실행
npm run dev
```

브라우저에서 자동으로 `http://localhost:5173`이 열립니다.

## ✨ 주요 기능

- 🔍 **검색 기능**: 주차 제목과 내용을 빠르게 검색
- 🌙 **다크 모드**: 시스템 테마 자동 감지 및 수동 전환
- 📱 **반응형 디자인**: 모바일, 태블릿, 데스크탑 모두 지원
- 💎 **Markdown 완벽 렌더링**:
  - 코드 하이라이팅 (C++, PowerShell 등)
  - Mermaid 다이어그램
  - GitHub Alerts (WARNING, IMPORTANT, NOTE 등)
  - 표, 인용구 등 모든 Markdown 요소
- ⚡ **빠른 성능**: Vite 기반 초고속 로딩

## 🛠️ 기술 스택

- React 18 + TypeScript
- Vite (빌드 도구)
- Tailwind CSS (스타일링)
- react-markdown (Markdown 렌더링)
- react-syntax-highlighter (코드 하이라이팅)
- Mermaid.js (다이어그램)

## 📁 자세한 내용

더 자세한 사용법과 프로젝트 구조는 `viewer/README.md`를 참고하세요.
