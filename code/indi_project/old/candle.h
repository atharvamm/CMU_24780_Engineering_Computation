
#ifndef CANDLE_H
#define CANDLE_H

class Candle{
    private:
        float sx,sy;
        float mainBody[6][2],fireBody[12][2],wick[6][2];
        bool fireVisible,objVisible;

    public:
        Candle(float,float);
        bool getVisibility();
        void switchVisibility();
        void fireVisbility();
        void fireAnimation();
        void fireStatic();
        void drawCandle();
};

#endif


        // float sx,sy;
        // float mainBody[6][2],
        // mainBodyHelper[6][2] = {{6, 25}, {-6, 25}, {6, -25}, {-6, -25}, {6, -35}, {6, -45}},wick[6][2],
        // wickHelper[6][2] = {{1, - 37},{-2,-37 + 14/2},{1,-41},{-2,-41},{1,-45},{-2,-43}}, fireBody[12][2],
        // fireHelper[12][2] = {{3,-40},{-3,-40},{5,-50},{-5,-51},{6,-52},{-3,-53},{5,-54},{-1,-55},{3,-56},{-1,-57},{0,-58},{-5,-61}};
        // bool fireVisible,objVisible;