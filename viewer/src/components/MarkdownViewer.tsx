import React, { useEffect, useId, useMemo, useRef } from 'react';
import type { ReactNode } from 'react';
import Markdoc from '@markdoc/markdoc';
import { Prism as SyntaxHighlighter } from 'react-syntax-highlighter';
import { atomDark } from 'react-syntax-highlighter/dist/esm/styles/prism';

type MarkdownViewerProps = {
  content: string;
  isLoading: boolean;
  title: string;
};

type FenceProps = {
  content: string;
  language?: string;
};

type InlineCodeProps = {
  children?: ReactNode;
  content?: string;
};

type HeadingProps = {
  id?: string;
  level: number;
  children?: ReactNode;
};

const slugify = (value?: ReactNode): string => {
  const text = React.Children.toArray(value ?? '')
    .map((child) => {
      if (typeof child === 'string') return child;
      if (typeof child === 'number') return String(child);
      return '';
    })
    .join(' ');
  return text
    .toLowerCase()
    .trim()
    .replace(/[^a-z0-9\s-]/g, '')
    .replace(/\s+/g, '-')
    .replace(/-+/g, '-');
};

const MARKDOC_CONFIG = {
  nodes: {
    fence: {
      render: 'Fence',
      attributes: {
        content: { type: String },
        language: { type: String }
      }
    },
    code: {
      render: 'InlineCode',
      attributes: {
        content: { type: String }
      }
    },
    heading: {
      render: 'Heading',
      attributes: {
        id: { type: String },
        level: { type: Number, required: true }
      }
    },
    table: {
      render: 'Table'
    }
  }
};

const MermaidDiagram = ({ code }: { code: string }) => {
  const containerRef = useRef<HTMLDivElement>(null);
  const reactId = useId().replace(/:/g, '-');

  useEffect(() => {
    let canceled = false;
    const renderDiagram = async () => {
      const { default: mermaid } = await import('mermaid');
      mermaid.initialize({ startOnLoad: false, theme: 'dark' });
      if (!containerRef.current || canceled) return;
      try {
        const { svg } = await mermaid.render(`mermaid-${reactId}`, code);
        if (!canceled && containerRef.current) {
          containerRef.current.innerHTML = svg;
        }
      } catch (error) {
        if (containerRef.current) {
          containerRef.current.innerHTML = `<pre class="text-red-400">Mermaid 렌더링 실패: ${String(error)}</pre>`;
        }
      }
    };

    renderDiagram();
    return () => {
      canceled = true;
      if (containerRef.current) {
        containerRef.current.innerHTML = '';
      }
    };
  }, [code, reactId]);

  return (
    <div
      ref={containerRef}
      className="mermaid-diagram my-6 rounded-2xl border border-white/10 bg-slate-950/70 p-4 text-slate-100 shadow-[0_12px_40px_rgba(2,6,23,0.45)]"
    />
  );
};

const CodeFence = ({ content, language }: FenceProps) => {
  const normalizedCode = content?.replace(/\n$/, '') ?? '';
  if ((language ?? '').toLowerCase() === 'mermaid') {
    return <MermaidDiagram code={normalizedCode} />;
  }

  const label = language ? language.toUpperCase() : undefined;
  return (
    <div className="md-code-block" data-language={label}>
      <SyntaxHighlighter
        language={language ?? 'text'}
        style={atomDark}
        PreTag="div"
        showLineNumbers
        customStyle={{ margin: 0, borderRadius: '1.25rem', background: 'transparent' }}
      >
        {normalizedCode}
      </SyntaxHighlighter>
    </div>
  );
};

const InlineCode = ({ children, content }: InlineCodeProps) => (
  <code className="rounded-md bg-white/10 px-2 py-0.5 text-[0.95em] font-medium text-slate-100 shadow-[0_0_0_1px_rgba(148,163,184,0.35)]">
    {children ?? content}
  </code>
);

const Heading = ({ children, id, level }: HeadingProps) => {
  const Tag = (`h${Math.min(level, 6)}` as keyof JSX.IntrinsicElements);
  const headingId = id || slugify(children) || undefined;
  return (
    <Tag id={headingId} className="group relative scroll-mt-24">
      {headingId && (
        <a
          href={`#${headingId}`}
          className="absolute -left-6 top-1/2 hidden -translate-y-1/2 text-accent opacity-0 transition group-hover:opacity-100 md:inline"
          aria-label="제목 앵커"
        >
          #
        </a>
      )}
      {children}
    </Tag>
  );
};

const Table = ({ children }: { children: ReactNode }) => (
  <div className="md-table-wrapper -mx-4 overflow-x-auto px-4">
    <table className="w-full table-auto border-collapse text-sm text-slate-100">{children}</table>
  </div>
);

const MarkdownViewer = ({ content, isLoading, title }: MarkdownViewerProps) => {
  const renderedMarkdown = useMemo(() => {
    if (!content?.trim()) return null;
    try {
      const ast = Markdoc.parse(content);
      const transformed = Markdoc.transform(ast, MARKDOC_CONFIG);
      return Markdoc.renderers.react(transformed, React, {
        components: {
          Fence: CodeFence,
          InlineCode,
          Heading,
          Table
        }
      });
    } catch (error) {
      return (
        <div className="rounded-2xl border border-rose-500/40 bg-rose-950/40 p-4 text-sm text-rose-100">
          모던 마크다운 변환에 실패했습니다.
          <span className="mt-2 block text-xs opacity-80">{String(error)}</span>
        </div>
      );
    }
  }, [content]);

  if (!content?.trim()) {
    return (
      <div className="flex h-full flex-col items-center justify-center rounded-[28px] border border-dashed border-white/15 bg-slate-950/40 p-8 text-slate-400">
        <span className="text-sm font-medium">
          {isLoading ? 'README 불러오는 중...' : '주차 README를 선택하세요.'}
        </span>
      </div>
    );
  }

  return (
    <div className="relative">
      <article className="markdown-body prose prose-slate prose-invert relative mx-auto w-full max-w-none rounded-[32px] border border-white/10 bg-slate-950/80 px-12 py-12 shadow-[0_30px_120px_rgba(2,6,23,0.65)] ring-1 ring-white/5 backdrop-blur-xl md:px-16">
        <header className="mb-8 border-b border-white/10 pb-4">
          <p className="text-xs font-semibold uppercase tracking-[0.2em] text-slate-400">Weekly README</p>
          <h1 className="mt-3 text-3xl font-bold text-white">{title}</h1>
        </header>
        {renderedMarkdown}
      </article>
      {isLoading && (
        <div className="pointer-events-none absolute inset-0 flex items-center justify-center">
          <div className="flex items-center gap-3 rounded-2xl bg-white/90 px-4 py-2 text-sm font-medium text-slate-600 shadow-2xl ring-1 ring-slate-200 dark:bg-slate-800/90 dark:text-slate-100">
            <span className="h-4 w-4 animate-spin rounded-full border-2 border-slate-300 border-t-accent"></span>
            선택한 주차를 불러오는 중...
          </div>
        </div>
      )}
    </div>
  );
};

export default MarkdownViewer;
