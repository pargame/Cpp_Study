export interface Week {
    number: number;
    title: string;
}

export interface WeekData {
    week: number;
    title: string;
    content?: string;
}

export type Theme = 'light' | 'dark';
