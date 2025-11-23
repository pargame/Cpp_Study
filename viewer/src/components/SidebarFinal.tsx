import { useState } from 'react';
import { Week } from '../types';
// import { SearchBar } from './SearchBar';
import { WeekCard } from './WeekCard';
// import { BookOpen } from 'lucide-react';

interface SidebarProps {
    weeks: Week[];
    selectedWeek: number | null;
    onSelectWeek: (weekNumber: number) => void;
}

export function SidebarFinal({ weeks, selectedWeek, onSelectWeek }: SidebarProps) {
    const [searchTerm, setSearchTerm] = useState('');

    const filteredWeeks = weeks.filter(week =>
        week.title.toLowerCase().includes(searchTerm.toLowerCase())
    );

    return (
        <div className="w-80 h-screen bg-gray-100 dark:bg-gray-800 border-r border-gray-300 dark:border-gray-700 flex flex-col transition-colors duration-200">
            <div className="p-6 border-b border-gray-300 dark:border-gray-700 bg-white dark:bg-gray-800">
                <div className="flex items-center gap-3 mb-6">
                    <div className="p-2 bg-blue-600 rounded-lg shadow-lg">
                        {/* <BookOpen className="w-6 h-6 text-white" /> */}
                        <span className="text-white font-bold">B</span>
                    </div>
                    <h1 className="text-xl font-bold text-gray-800 dark:text-white tracking-tight">
                        C++ Study
                    </h1>
                </div>
                {/* <SearchBar value={searchTerm} onChange={setSearchTerm} /> */}
            </div>

            <div className="flex-1 overflow-y-auto p-4 space-y-2 custom-scrollbar">
                {filteredWeeks.length === 0 ? (
                    <div className="text-center py-8 text-gray-500 dark:text-gray-400">
                        <p>No weeks found</p>
                    </div>
                ) : (
                    filteredWeeks.map((week) => (
                        <div
                            key={week.number}
                            onClick={() => onSelectWeek(week.number)}
                            className="p-2 cursor-pointer hover:bg-gray-200 dark:hover:bg-gray-700"
                        >
                            {week.title}
                        </div>
                    ))
                )}
            </div>
        </div>
    );
}
