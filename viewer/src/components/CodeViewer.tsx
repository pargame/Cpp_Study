import { useEffect, useState } from 'react';
import { Prism as SyntaxHighlighter } from 'react-syntax-highlighter';
import { coldarkDark } from 'react-syntax-highlighter/dist/esm/styles/prism';
import type { WeekSourceFile } from '../data/weeks.generated';

type CodeViewerProps = {
  file: WeekSourceFile | null;
  isLoading: boolean;
  error: string | null;
  content: string;
  onRetry?: () => void;
};

const fallbackTitle = '소스코드를 선택하세요';

const CopyButton = ({ text }: { text: string }) => {
  const [copied, setCopied] = useState(false);

  useEffect(() => {
    if (!copied) return;
    const timer = setTimeout(() => setCopied(false), 1500);
    return () => clearTimeout(timer);
  }, [copied]);

  const handleCopy = async () => {
    if (!text || typeof navigator === 'undefined' || !navigator.clipboard) return;
    try {
      await navigator.clipboard.writeText(text);
      setCopied(true);
    } catch (copyError) {
      console.error('클립보드 복사에 실패했습니다.', copyError);
    }
  };

  return (
    <button
      type="button"
      onClick={handleCopy}
      className="rounded-full border border-slate-600 px-3 py-1 text-xs font-medium text-slate-200 transition hover:border-indigo-400 hover:text-white"
    >
      {copied ? '복사됨' : '복사'}
    </button>
  );
};

const CodeViewer = ({ file, isLoading, error, content, onRetry }: CodeViewerProps) => {
  if (!file) {
    return (
      <div className="flex h-full flex-col items-center justify-center rounded-3xl border border-dashed border-slate-700/60 bg-slate-900/40 p-6 text-sm text-slate-400">
        {fallbackTitle}
      </div>
    );
  }

  if (error) {
    return (
      <div className="rounded-3xl border border-red-500/40 bg-red-900/30 p-4 text-sm text-red-200">
        <p className="font-semibold">소스코드를 불러오지 못했습니다.</p>
        <p className="mt-2">{error}</p>
        {onRetry && (
          <button
            type="button"
            onClick={onRetry}
            className="mt-4 rounded-full border border-red-300/60 px-4 py-1 text-xs font-semibold text-red-100 transition hover:border-red-200"
          >
            다시 시도
          </button>
        )}
      </div>
    );
  }

  const mergedTheme = {
    ...coldarkDark,
    hljs: {
      ...coldarkDark.hljs,
      background: 'transparent'
    }
  };

  return (
    <div className="flex h-full flex-col rounded-3xl border border-slate-800/80 bg-slate-950/80 p-5 shadow-xl">
      <header className="flex items-center justify-between gap-3 border-b border-slate-800/70 pb-3">
        <div>
          <p className="text-[11px] uppercase tracking-[0.4em] text-slate-500">Source</p>
          <h3 className="font-mono text-sm text-slate-100">{file.name}</h3>
        </div>
        <CopyButton text={content} />
      </header>
      <div className="flex-1 overflow-auto pt-4">
        {isLoading ? (
          <div className="flex h-full items-center justify-center text-sm text-slate-400">로드 중...</div>
        ) : (
          <SyntaxHighlighter
            language={file.language === 'text' ? undefined : file.language}
            style={mergedTheme}
            customStyle={{
              background: 'transparent',
              margin: 0,
              padding: '0.5rem 1rem 1.25rem',
              borderRadius: '1rem',
              fontSize: '0.88rem'
            }}
            showLineNumbers
            wrapLongLines
          >
            {content || '// 내용이 비어 있습니다.'}
          </SyntaxHighlighter>
        )}
      </div>
    </div>
  );
};

export default CodeViewer;
