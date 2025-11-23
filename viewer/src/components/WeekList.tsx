import type { WeekMeta } from '../data/weeks.generated';

type WeekListProps = {
  items: WeekMeta[];
  selectedId: string | null;
  onSelect: (week: WeekMeta) => void;
  isLoading: boolean;
};

const WeekList = ({ items, selectedId, onSelect, isLoading }: WeekListProps) => {
  if (!items.length) {
    return (
      <p className="rounded-2xl border border-dashed border-slate-700/70 bg-slate-900/40 p-4 text-sm text-slate-400">
        검색된 주차가 없습니다.
      </p>
    );
  }

  return (
    <ul className="space-y-1">
      {items.map((week) => {
        const isActive = week.id === selectedId;
        return (
          <li key={week.id}>
            <button
              type="button"
              onClick={() => onSelect(week)}
              disabled={isLoading && !isActive}
              aria-pressed={isActive}
              className={`group w-full rounded-2xl border px-4 py-3 text-left transition ${
                isActive
                  ? 'border-indigo-500/60 bg-indigo-500/10 text-white shadow-lg'
                  : 'border-transparent bg-slate-900/40 text-slate-300 hover:border-slate-700 hover:bg-slate-900/70'
              }`}
            >
              <div className="flex items-center justify-between text-xs font-semibold uppercase tracking-[0.25em] text-slate-500">
                <span>{week.id}</span>
                {isActive && isLoading && (
                  <span className="flex items-center gap-1 text-[11px] tracking-normal text-indigo-300">
                    <span className="h-2.5 w-2.5 animate-ping rounded-full bg-current/60" />
                    로딩중
                  </span>
                )}
              </div>
              <h3 className="text-sm font-semibold text-slate-100">{week.title}</h3>
              {/* intentionally left blank per design (no summary) */}
            </button>
          </li>
        );
      })}
    </ul>
  );
};

export default WeekList;
