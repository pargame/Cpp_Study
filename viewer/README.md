# C++ Study README Viewer

Week1~Week32까지 32개 주차의 C++ 멀티쓰레딩 & 네트워크 프로그래밍 커리큘럼을 탐색할 수 있는 모던한 React 웹앱입니다.

## 🚀 기능

- 📚 **32주차 커리큘럼 탐색**: Week1부터 Week32까지 모든 주차의 README를 한 곳에서
- 🔍 **실시간 검색**: 주차 제목과 내용을 빠르게 검색
- 🌙 **다크 모드**: 눈이 편한 다크 테마 지원
- 💎 **Markdown 완벽 렌더링**: 코드 하이라이팅, Mermaid 다이어그램, GitHub Alerts
- 🎨 **모던한 UI**: Tailwind CSS 기반의 깔끔하고 반응형 디자인
- ⚡ **빠른 성능**: Vite의 초고속 HMR과 최적화된 빌드

## 📦 기술 스택

- **Frontend**: React 18 + TypeScript
- **Build Tool**: Vite
- **Styling**: Tailwind CSS
- **Markdown**: react-markdown + remark-gfm
- **코드 하이라이팅**: react-syntax-highlighter
- **다이어그램**: Mermaid.js
- **아이콘**: Lucide React

## 🛠️ 설치 및 실행

### 개발 모드

```powershell
cd e:\repos\C++_Study\viewer
npm run dev
```

브라우저가 자동으로 `http://localhost:5173`에서 열립니다.

### 프로덕션 빌드

```powershell
npm run build
npm run preview
```

## 📁 프로젝트 구조

```
viewer/
├── src/
│   ├── components/          # React 컴포넌트
│   │   ├── Sidebar.tsx      # 주차 목록 사이드바
│   │   ├── MarkdownViewer.tsx  # README 렌더링
│   │   ├── SearchBar.tsx    # 검색 입력
│   │   ├── ThemeToggle.tsx  # 다크모드 토글
│   │   └── WeekCard.tsx     # 주차 카드
│   ├── hooks/               # 커스텀 훅
│   │   ├── useTheme.ts      # 테마 관리
│   │   └── useMarkdown.ts   # Markdown 로딩
│   ├── types/               # TypeScript 타입
│   │   └── index.ts
│   ├── App.tsx              # 메인 앱
│   ├── main.tsx             # 엔트리 포인트
│   └── index.css            # 전역 스타일
├── public/                  # 정적 파일
├── tailwind.config.js       # Tailwind 설정
├── vite.config.ts           # Vite 설정
└── package.json
```

## 🎯 사용법

1. **주차 선택**: 왼쪽 사이드바에서 원하는 주차를 클릭
2. **검색**: 상단 검색창에서 주차 제목이나 번호로 검색
3. **다크모드**: 우측 상단의 토글 버튼으로 테마 변경
4. **URL 공유**: `http://localhost:5173/#/week/5` 형식으로 특정 주차 직접 접근

## 🎨 주요 기능 상세

### Markdown 렌더링
- GitHub Flavored Markdown (GFM) 완벽 지원
- 코드 블록 자동 하이라이팅 (C++, PowerShell 등)
- Mermaid 다이어그램 렌더링
- GitHub Alerts (NOTE, TIP, IMPORTANT, WARNING, CAUTION)
- 표(Table), 인용구(Blockquote) 등 모든 Markdown 요소 지원

### 다크 모드
- 시스템 테마 자동 감지
- 사용자 선호도 localStorage에 저장
- 부드러운 전환 애니메이션

### 검색 기능
- 실시간 필터링
- 주차 번호 및 제목 검색
- 대소문자 구분 없음

## 🔧 문제 해결

### README 파일이 로드되지 않는 경우
Vite dev 서버는 상위 디렉토리 접근을 허용하도록 설정되어 있습니다. 만약 문제가 있다면 `vite.config.ts`의 `server.fs.allow` 설정을 확인하세요.

### 다크 모드가 작동하지 않는 경우
브라우저의 localStorage를 확인하고, 필요시 개발자 도구에서 `localStorage.clear()`를 실행하세요.

## 📝 라이선스

이 프로젝트는 C++ Study 커리큘럼의 일부입니다.
