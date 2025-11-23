# C++ Study Viewer

가독성 좋은 인터페이스로 Week1 ~ Week32 README와 실제 예제 소스코드를 탐색할 수 있는 정적 웹앱입니다. Vite + React + Tailwind 기반이며 Markdown/코드 모두 어두운 테마에 최적화되어 있습니다.

## 주요 기능

- 자동 동기화: 각 Week 디렉터리의 README와 `src/` 내 모든 파일을 정적 자산으로 복사
- 검색/필터링과 주차별 README 렌더링 (Mermaid/코드 하이라이트 지원)
- 다크 모드 고정 UI (토글 없음)
- 주차별 소스코드 뷰어: 파일 리스트 + 구문 하이라이트 + 원클릭 복사

## 사용 방법

```powershell
cd viewer
npm install
npm run dev
```

빌드 산출물은 `npm run build`로 생성되며, `dist/` 디렉터리를 정적 호스팅에 업로드하면 됩니다.

README 목록을 갱신하려면 `npm run sync`로 원본 Week README를 다시 복사하세요. `npm run dev` 및 `npm run build` 실행 전에 자동으로 동기화가 수행됩니다.
