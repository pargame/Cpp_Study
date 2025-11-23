import { useEffect, useMemo, useRef, useState } from 'react';
import SearchInput from './components/SearchInput';
import WeekList from './components/WeekList';
import MarkdownViewer from './components/MarkdownViewer';
import SourceList from './components/SourceList';
import CodeViewer from './components/CodeViewer';
import { weeks, type WeekMeta, type WeekSourceFile } from './data/weeks.generated';

const SHOULD_CACHE_RESPONSES = import.meta.env.MODE !== 'development';

const isAbortError = (error: unknown): error is DOMException =>
  error instanceof DOMException && error.name === 'AbortError';

const asErrorMessage = (error: unknown, fallback: string) => (error instanceof Error ? error.message : fallback);

const App = () => {
  const [query, setQuery] = useState('');
  const [selectedWeek, setSelectedWeek] = useState<WeekMeta | null>(weeks[0] ?? null);
  const [markdown, setMarkdown] = useState('');
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);
  const [selectedSource, setSelectedSource] = useState<WeekSourceFile | null>(null);
  const [sourceContent, setSourceContent] = useState('');
  const [sourceLoading, setSourceLoading] = useState(false);
  const [sourceError, setSourceError] = useState<string | null>(null);
  const [isDrawerOpen, setIsDrawerOpen] = useState(false);
  const [weekReloadToken, setWeekReloadToken] = useState(0);
  const [sourceReloadToken, setSourceReloadToken] = useState(0);
  const markdownCacheRef = useRef<Map<string, string>>(new Map());
  const sourceCacheRef = useRef<Map<string, string>>(new Map());

  const handleWeekRetry = () => {
    if (!selectedWeek) return;
    markdownCacheRef.current.delete(selectedWeek.path);
    setError(null);
    setWeekReloadToken((token) => token + 1);
  };

  const handleSourceRetry = () => {
    if (!selectedSource) return;
    sourceCacheRef.current.delete(selectedSource.path);
    setSourceError(null);
    setSourceReloadToken((token) => token + 1);
  };

  const filteredWeeks = useMemo(() => {
    const normalized = query.trim().toLowerCase();
    if (!normalized) {
      return weeks;
    }
    return weeks.filter((week) => {
      const haystack = `${week.id} ${week.title}`.toLowerCase();
      return haystack.includes(normalized);
    });
  }, [query]);

  useEffect(() => {
    if (!selectedWeek) {
      setMarkdown('');
      setLoading(false);
      return;
    }

    const targetWeek = selectedWeek;
    const cacheKey = targetWeek.path;
    setError(null);

    const cached = SHOULD_CACHE_RESPONSES ? markdownCacheRef.current.get(cacheKey) : undefined;
    if (cached) {
      setMarkdown(cached);
      setLoading(false);
      return;
    }

    const controller = new AbortController();
    let canceled = false;
    setLoading(true);

    fetch(cacheKey, { signal: controller.signal })
      .then((response) => {
        if (!response.ok) {
          throw new Error(`${targetWeek.id} README를 불러오지 못했습니다.`);
        }
        return response.text();
      })
      .then((text) => {
        if (canceled) return;
        if (SHOULD_CACHE_RESPONSES) {
          markdownCacheRef.current.set(cacheKey, text);
        }
        setMarkdown(text);
      })
      .catch((err: unknown) => {
        if (canceled || isAbortError(err)) return;
        setError(asErrorMessage(err, `${targetWeek.id} README를 불러오지 못했습니다.`));
      })
      .finally(() => {
        if (!canceled) {
          setLoading(false);
        }
      });

    return () => {
      canceled = true;
      controller.abort();
    };
  }, [selectedWeek, weekReloadToken]);

  useEffect(() => {
    if (!selectedWeek || !selectedWeek.sources.length) {
      setSelectedSource(null);
      setSourceContent('');
      setSourceError(null);
      setIsDrawerOpen(false);
      return;
    }
    setSelectedSource(selectedWeek.sources[0]);
  }, [selectedWeek]);

  useEffect(() => {
    if (!selectedSource) {
      setSourceContent('');
      setSourceError(null);
      setSourceLoading(false);
      return;
    }

    const targetSource = selectedSource;
    const cacheKey = targetSource.path;
    setSourceError(null);

    const cached = SHOULD_CACHE_RESPONSES ? sourceCacheRef.current.get(cacheKey) : undefined;
    if (cached) {
      setSourceContent(cached);
      setSourceLoading(false);
      return;
    }

    const controller = new AbortController();
    let canceled = false;
    setSourceLoading(true);

    fetch(cacheKey, { signal: controller.signal })
      .then((response) => {
        if (!response.ok) {
          throw new Error(`${targetSource.name} 파일을 불러오지 못했습니다.`);
        }
        return response.text();
      })
      .then((text) => {
        if (canceled) return;
        if (SHOULD_CACHE_RESPONSES) {
          sourceCacheRef.current.set(cacheKey, text);
        }
        setSourceContent(text);
      })
      .catch((err: unknown) => {
        if (canceled || isAbortError(err)) return;
        setSourceError(asErrorMessage(err, `${targetSource.name} 파일을 불러오지 못했습니다.`));
      })
      .finally(() => {
        if (!canceled) {
          setSourceLoading(false);
        }
      });

    return () => {
      canceled = true;
      controller.abort();
    };
  }, [selectedSource, sourceReloadToken]);

  useEffect(() => {
    if (!selectedWeek) {
      return;
    }
    const stillVisible = filteredWeeks.some((week) => week.id === selectedWeek.id);
    if (!stillVisible) {
      setSelectedWeek(filteredWeeks[0] ?? null);
    }
  }, [filteredWeeks, selectedWeek]);

  const hasSources = Boolean(selectedWeek?.sources.length);

  return (
    <div className="min-h-screen bg-gradient-to-br from-slate-950 via-slate-900 to-slate-950 p-6 text-slate-100">
      <div className="mx-auto flex w-full max-w-[1800px] flex-col gap-6 lg:flex-row">
        <aside className="w-full lg:max-w-sm">
          <div className="rounded-3xl border border-slate-800 bg-slate-900/70 p-6 shadow-2xl">
            <header>
              <p className="text-[11px] font-semibold uppercase tracking-[0.4em] text-slate-500">Curriculum</p>
              <h1 className="mt-2 text-3xl font-bold text-white">C++ Study Viewer</h1>
              <p className="mt-3 text-sm text-slate-400">주차별 README와 실제 예제 소스코드를 한 화면에서 확인하세요.</p>
            </header>
            <div className="mt-6 space-y-4">
              <SearchInput value={query} onChange={setQuery} />
              <div className="max-h-[70vh] overflow-y-auto pr-2">
                <WeekList
                  items={filteredWeeks}
                  selectedId={selectedWeek?.id ?? null}
                  onSelect={(week) => setSelectedWeek(week)}
                  isLoading={loading}
                />
              </div>
            </div>
          </div>
        </aside>
        <main className="flex-1 space-y-6">
          {error && (
            <div className="rounded-2xl border border-red-500/40 bg-red-900/30 p-6 text-red-100">
              <h2 className="text-lg font-semibold">로드 오류</h2>
              <p className="mt-2 text-sm">{error}</p>
              <button
                type="button"
                className="mt-4 rounded-lg border border-red-400/50 px-3 py-1 text-sm font-medium text-red-100"
                onClick={handleWeekRetry}
              >
                다시 시도
              </button>
            </div>
          )}
          <section className="rounded-3xl border border-slate-800/70 bg-slate-900/60 p-6 shadow-2xl">
            <div className="flex flex-wrap items-end justify-between gap-4">
              <div>
                <p className="text-[11px] uppercase tracking-[0.4em] text-slate-500">Reading Mode</p>
                <h2 className="text-3xl font-semibold text-white">README를 먼저 정독하세요</h2>
                <p className="mt-2 text-sm text-slate-400">
                  {selectedWeek ? `${selectedWeek.id} · ${selectedWeek.title}` : '확인할 주차를 먼저 선택하세요.'}
                </p>
              </div>
              <button
                type="button"
                aria-expanded={isDrawerOpen}
                onClick={() => hasSources && setIsDrawerOpen(true)}
                disabled={!hasSources}
                className={`rounded-full px-5 py-2 text-sm font-semibold transition ${
                  hasSources
                    ? 'border border-indigo-400/60 bg-indigo-500/10 text-indigo-100 hover:border-indigo-300'
                    : 'border border-slate-700/70 bg-slate-800/60 text-slate-500'
                }`}
              >
                {hasSources ? 'C++ 소스 서랍 열기' : '공유된 소스 없음'}
              </button>
            </div>
            <div className="mt-6">
              <MarkdownViewer content={markdown} isLoading={loading} title={selectedWeek?.title ?? 'README 미선택'} />
            </div>
          </section>
        </main>
      </div>
      {isDrawerOpen && (
        <div
          className="fixed inset-0 z-30 bg-slate-950/70 backdrop-blur-sm"
          role="presentation"
          onClick={() => setIsDrawerOpen(false)}
        />
      )}
      <aside
        className={`fixed inset-y-0 right-0 z-40 w-full max-w-[90vw] overflow-hidden border-l border-slate-800/80 bg-slate-950/95 shadow-2xl transition-transform duration-300 ease-in-out md:max-w-[82vw] lg:max-w-[74vw] xl:max-w-[68vw] 2xl:max-w-[60vw] ${
          isDrawerOpen ? 'translate-x-0' : 'translate-x-full'
        }`}
        aria-hidden={!isDrawerOpen}
      >
        <div className="flex h-full flex-col overflow-hidden">
          <header className="flex items-start justify-between gap-4 border-b border-slate-800/80 p-6">
            <div>
              <p className="text-[11px] uppercase tracking-[0.4em] text-slate-500">Source Drawer</p>
              <h2 className="text-2xl font-semibold text-white">{selectedWeek?.id ?? 'Week ?'}</h2>
              <p className="mt-2 text-sm text-slate-400">README를 벗어나지 않고 필요한 코드만 살펴보세요.</p>
            </div>
            <button
              type="button"
              onClick={() => setIsDrawerOpen(false)}
              className="rounded-full border border-slate-700/70 px-4 py-2 text-sm font-semibold text-slate-200 transition hover:border-slate-500"
            >
              닫기
            </button>
          </header>
          <div className="flex flex-1 flex-col gap-4 overflow-hidden p-6 lg:grid lg:grid-cols-[320px,1fr]">
            <div className="max-h-[65vh] overflow-y-auto pr-2">
              <SourceList
                files={selectedWeek?.sources ?? []}
                selectedPath={selectedSource?.path ?? null}
                onSelect={(file) => setSelectedSource(file)}
                disabled={sourceLoading}
              />
            </div>
            <div className="min-h-[60vh] overflow-y-auto">
              <CodeViewer
                file={selectedSource}
                isLoading={sourceLoading}
                error={sourceError}
                content={sourceContent}
                onRetry={handleSourceRetry}
              />
            </div>
          </div>
        </div>
      </aside>
    </div>
  );
};

export default App;
