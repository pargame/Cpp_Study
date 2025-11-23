import { Week } from '../types';

interface WeekCardProps {
    week: Week;
    isActive: boolean;
    onClick: () => void;
}

export function WeekCard({ week, isActive, onClick }: WeekCardProps) {
    return (
        <button
            onClick={onClick}
            className={`
        w-full text-left p-3 rounded-lg transition-all duration-200
        ${isActive
                    ? 'bg-primary-500 text-white shadow-lg'
                    : 'bg-white dark:bg-gray-800 hover:bg-gray-100 dark:hover:bg-gray-700'
                }
      `}
        >
            <div className="font-semibold text-sm">
                Week {week.number}
            </div>
            <div className={`text-xs mt-1 line-clamp-1 ${isActive ? 'text-white/90' : 'text-gray-600 dark:text-gray-400'}`}>
                {week.title}
            </div>
        </button>
    );
}
