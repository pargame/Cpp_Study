import { useState, useEffect } from 'react';

// --- Types ---
interface Week {
    number: number;
    title: string;
}

type Theme = 'light' | 'dark';

// --- Inline Components ---

function ThemeToggle({ theme, toggleTheme }: { theme: Theme; toggleTheme: () => void }) {
    return (
        <button
            onClick={toggleTheme}
            className="p-2 rounded-lg bg-gray-200 dark:bg-gray-700 hover:bg-gray-300 dark:hover:bg-gray-600 transition-colors"
            aria-label="Toggle theme"
        >
            {theme === 'light' ? (
                <span className="text-yellow-500 font-bold">🌙</span>
            ) : (
                <span className="text-blue-400 font-bold">☀️</span>
            )}
        </button>
    );
}

function SearchBar({ value, onChange }: { value: string; onChange: (val: string) => void }) {
    return (
        <div className="relative">
            <input
                type="text"
                value={value}
                onChange={(e) => onChange(e.target.value)}
                placeholder="Search weeks..."
                className="w-full px-4 py-2 pl-10 bg-gray-100 dark:bg-gray-700 border border-gray-300 dark:border-gray-600 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500 text-gray-900 dark:text-gray-100 placeholder-gray-500 dark:placeholder-gray-400 transition-colors"
            />
            <div className="absolute left-3 top-2.5 text-gray-400">
                🔍
            </div>
        </div>
    );
}

function WeekCard({ week, isSelected, onClick }: { week: Week; isSelected: boolean; onClick: () => void }) {
    return (
        <div
            onClick={onClick}
            className={`
                group flex items-center justify-between p-3 rounded-lg cursor-pointer transition-all duration-200
                ${isSelected
                    ? 'bg-blue-600 text-white shadow-md transform scale-[1.02]'
                    : 'hover:bg-gray-200 dark:hover:bg-gray-700 text-gray-700 dark:text-gray-300'
                }
            `}
        >
            <span className="font-medium truncate">{week.title}</span>
            {isSelected && (
                <span className="text-white">👉</span>
            )}
        </div>
    );
}

function Sidebar({ weeks, selectedWeek, onSelectWeek }: { weeks: Week[]; selectedWeek: number | null; onSelectWeek: (n: number) => void }) {
    const [searchTerm, setSearchTerm] = useState('');

    const filteredWeeks = weeks.filter(week =>
        week.title.toLowerCase().includes(searchTerm.toLowerCase())
    );

    return (
        <div className="w-80 h-screen bg-gray-100 dark:bg-gray-800 border-r border-gray-300 dark:border-gray-700 flex flex-col transition-colors duration-200">
            <div className="p-6 border-b border-gray-300 dark:border-gray-700 bg-white dark:bg-gray-800">
                <div className="flex items-center gap-3 mb-6">
                    <div className="p-2 bg-blue-600 rounded-lg shadow-lg">
                        <span className="text-white font-bold text-xl">📚</span>
                    </div>
                    <h1 className="text-xl font-bold text-gray-800 dark:text-white tracking-tight">
                        C++ Study
                    </h1>
                </div>
                <SearchBar value={searchTerm} onChange={setSearchTerm} />
            </div>

            <div className="flex-1 overflow-y-auto p-4 space-y-2">
                {filteredWeeks.length === 0 ? (
                    <div className="text-center py-8 text-gray-500 dark:text-gray-400">
                        <p>No weeks found</p>
                    </div>
                ) : (
                    filteredWeeks.map((week) => (
                        <WeekCard
                            key={week.number}
                            week={week}
                            isSelected={selectedWeek === week.number}
                            onClick={() => onSelectWeek(week.number)}
                        />
                    ))
                )}
            </div>
        </div>
    );
}

// --- Main App ---

const weeks: Week[] = Array.from({ length: 32 }, (_, i) => ({
    number: i + 1,
    title: `Week ${i + 1}`,
}));

function AppNew() {
    const [selectedWeek, setSelectedWeek] = useState<number | null>(null);
    const [theme, setTheme] = useState<Theme>('light');

    // Theme management
    useEffect(() => {
        const savedTheme = localStorage.getItem('theme') as Theme | null;
        const prefersDark = window.matchMedia('(prefers-color-scheme: dark)').matches;
        const initialTheme = savedTheme || (prefersDark ? 'dark' : 'light');
        setTheme(initialTheme);
        document.documentElement.classList.toggle('dark', initialTheme === 'dark');
    }, []);

    const toggleTheme = () => {
        const newTheme = theme === 'light' ? 'dark' : 'light';
        setTheme(newTheme);
        localStorage.setItem('theme', newTheme);
        document.documentElement.classList.toggle('dark', newTheme === 'dark');
    };

    const handleSelectWeek = (weekNumber: number) => {
        setSelectedWeek(weekNumber);
    };

    return (
        <div className="flex h-screen overflow-hidden bg-white dark:bg-gray-900 text-gray-900 dark:text-gray-100">
            <Sidebar
                weeks={weeks}
                selectedWeek={selectedWeek}
                onSelectWeek={handleSelectWeek}
            />

            <div className="flex-1 flex flex-col h-screen overflow-hidden">
                <header className="h-16 border-b border-gray-300 dark:border-gray-700 flex items-center justify-between px-6 bg-white dark:bg-gray-800 shrink-0">
                    <h2 className="text-lg font-semibold truncate">
                        {selectedWeek ? `Week ${selectedWeek}` : 'C++ 멀티쓰레딩 & 네트워크 프로그래밍'}
                    </h2>
                    <ThemeToggle theme={theme} toggleTheme={toggleTheme} />
                </header>

                <main className="flex-1 overflow-hidden relative">
                    <div className="absolute inset-0 overflow-y-auto p-8">
                        {selectedWeek ? (
                            <div>
                                <h1 className="text-3xl font-bold mb-4">Week {selectedWeek}</h1>
                                <p className="text-gray-600 dark:text-gray-400">
                                    README content for Week {selectedWeek} will be loaded here.
                                </p>
                            </div>
                        ) : (
                            <div>
                                <h1 className="text-3xl font-bold text-green-600 mb-4">Welcome to C++ Study!</h1>
                                <p className="text-gray-600 dark:text-gray-400">
                                    Select a week from the sidebar to view its README.
                                </p>
                            </div>
                        )}
                    </div>
                </main>
            </div>
        </div>
    );
}

export default AppNew;
