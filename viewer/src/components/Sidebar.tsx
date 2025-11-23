import { Week } from '../types';

interface SidebarProps {
    weeks: Week[];
    selectedWeek: number | null;
    onSelectWeek: (weekNumber: number) => void;
}

export function Sidebar({ weeks, selectedWeek, onSelectWeek }: SidebarProps) {
    return (
        <div className="w-80 h-screen bg-gray-100 dark:bg-gray-800 border-r border-gray-300 dark:border-gray-700 p-4 overflow-y-auto">
            <h1 className="text-xl font-bold mb-4">C++ Study</h1>
            <ul className="space-y-2">
                {weeks.map(week => (
                    <li
                        key={week.number}
                        onClick={() => onSelectWeek(week.number)}
                        className={`p-2 cursor-pointer ${selectedWeek === week.number ? 'bg-blue-500 text-white' : ''}`}
                    >
                        {week.title}
                    </li>
                ))}
            </ul>
        </div>
    );
}
