type SearchInputProps = {
  value: string;
  onChange: (value: string) => void;
};

const SearchInput = ({ value, onChange }: SearchInputProps) => {
  return (
    <label className="group flex items-center gap-2 rounded-xl border border-slate-200 bg-white/80 px-3 py-2 shadow-sm transition focus-within:border-accent dark:border-slate-700 dark:bg-slate-800/80">
      <svg
        viewBox="0 0 24 24"
        className="h-5 w-5 text-slate-400 group-focus-within:text-accent"
        fill="none"
        stroke="currentColor"
        strokeWidth="1.6"
        aria-hidden
      >
        <path d="M15.5 15.5L20 20" strokeLinecap="round" strokeLinejoin="round" />
        <circle cx="10.5" cy="10.5" r="6.5" />
      </svg>
      <input
        type="search"
        placeholder="주차 제목이나 내용을 검색하세요"
        value={value}
        onChange={(event) => onChange(event.target.value)}
        className="flex-1 bg-transparent text-sm text-slate-700 placeholder:text-slate-400 focus:outline-none dark:text-slate-200"
      />
    </label>
  );
};

export default SearchInput;
