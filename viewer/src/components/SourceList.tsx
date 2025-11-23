import type { WeekSourceFile } from '../data/weeks.generated';

type SourceListProps = {
  files: WeekSourceFile[];
  selectedPath: string | null;
  onSelect: (file: WeekSourceFile) => void;
  disabled?: boolean;
};

const SourceList = ({ files, selectedPath, onSelect, disabled = false }: SourceListProps) => {
  if (!files.length) {
    return (
      <p className="rounded-2xl border border-dashed border-slate-700/70 bg-slate-900/40 p-4 text-center text-sm text-slate-400">
        이 주차에는 공유된 소스코드가 없습니다.
      </p>
    );
  }

  return (
    <ul className="space-y-1 text-sm">
      {files.map((file) => {
        const isActive = file.path === selectedPath;
        return (
          <li key={file.path}>
            <button
              type="button"
              disabled={disabled && !isActive}
              aria-pressed={isActive}
              onClick={() => onSelect(file)}
              className={`group flex w-full items-center justify-between rounded-2xl border px-3 py-2 text-left transition ${
                isActive
                  ? 'border-indigo-400/60 bg-indigo-500/10 text-indigo-100 shadow-inner'
                  : 'border-transparent bg-slate-900/40 text-slate-200 hover:border-slate-700 hover:bg-slate-900/70'
              }`}
            >
              <span className="truncate font-mono text-xs">{file.name}</span>
              <span className="text-[10px] uppercase tracking-widest text-slate-500 group-hover:text-slate-300">
                {file.language}
              </span>
            </button>
          </li>
        );
      })}
    </ul>
  );
};

export default SourceList;
