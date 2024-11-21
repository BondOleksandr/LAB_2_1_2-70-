#include <raylib.h>
#include <iostream>
#include <string>
#include <random>

using namespace std;

struct controls{
    bool pause=true;
    bool dir=true;
    int framerate=60;
    int walk;
    int trck;
    int vall;
    bool selection = false;
};

class object {
public:
    virtual void Draw() const = 0;
    virtual ~object() = default;
};


class Text : public object {
private:
    const char* txt;
    int X;
    int Y;
    int FSize;
    Color CLR;

public:
    Text(const char TXT[], int x, int y, int fsize, Color clr): txt(TXT), X(x), Y(y), FSize(fsize), CLR(clr){};

    const char* getTxt() const { return txt; }
    int getX() const { return X; }
    int getY() const { return Y; }
    int getFSize() const { return FSize; }
    Color getColor() const { return CLR; }

    void setTxt(const char* TXT) { txt = TXT; }
    void setX(int x) { X = x; }
    void setY(int y) { Y = y; }
    void setFSize(int fsize) { FSize = fsize; }
    void setColor(Color clr) { CLR = clr; }

    void Draw() const override {
        DrawText(txt, X, Y, FSize, CLR);
    }
};

class RTangle : public object {
private:
    int X=0;
    int Y=0;
    int LGT=0;
    int HGT=0;
    Color CLR=RAYWHITE;

public:
    RTangle(int x, int y, int lgt, int hgt, Color clr) : X(x), Y(y), LGT(lgt), HGT(hgt), CLR(clr) {}

    // Геттери
    int getX() const { return X; }
    int getY() const { return Y; }
    int getLength() const { return LGT; }
    int getHeight() const { return HGT; }
    Color getColor() const { return CLR; }

    void setX(int x) { X = x; }
    void setY(int y) { Y = y; }
    void setLength(int lgt) { LGT = lgt; }
    void setHeight(int hgt) { HGT = hgt; }
    void setColor(Color clr) { CLR = clr; }

    void Draw() const override {
        DrawRectangle(X, Y, LGT, HGT, CLR);
    }
};

RTangle PAUSEdefRT(20, 750,200,100, GRAY);
Text PAUSEdefTXT("PAUSE", 38,777,50, BLACK);

RTangle CHNGDIRdefRT(240, 750,200,100, RED);
Text CHNGDIRdefTXT("RVS", 290,777,50, WHITE);

RTangle FPSINCdefRT(460, 750,200,100, ORANGE);
Text FPSINCdefTXT("FPS+", 510,777,50, WHITE);

RTangle FPSDECdefRT(680, 750,200,100, BLUE);
Text FPSDECdefTXT("FPS-", 725,777,50, WHITE);

RTangle WLKRdefRT(1120, 750,200,100, GREEN);
Text WLKRdefTXT(">>>>", 1155,777,50, WHITE);

RTangle WLKLdefRT(900, 750,200,100, GREEN);
Text WLKLdefTXT("<<<<", 940,777,50, WHITE);

RTangle SeldefRT(1340, 750,200,100, PURPLE);
Text SeldefTXT("SEL", 1370,777,50, WHITE);

RTangle DEFRT[7]={PAUSEdefRT, CHNGDIRdefRT, FPSINCdefRT, FPSDECdefRT, WLKRdefRT, WLKLdefRT, SeldefRT};
Text DEFTXT[7]={PAUSEdefTXT, CHNGDIRdefTXT, FPSINCdefTXT, FPSDECdefTXT, WLKRdefTXT, WLKLdefTXT, SeldefTXT};

class BUTTON {
protected:
    RTangle btn;
    Text inf;

    bool IsButtonClicked(Vector2 mousePosition) const {
        Rectangle check = {btn.getX(), btn.getY(), btn.getLength(), btn.getHeight()};
        return CheckCollisionPointRec(mousePosition, check) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    }

public:
    BUTTON(RTangle BTN, Text INF) : btn(BTN), inf(INF) {}

    virtual ~BUTTON() = default;

    virtual controls evaluate(controls A) = 0;

    virtual void Draw() {
        btn.Draw();
        inf.Draw();
    }

    void SetButton(const RTangle& newBtn, const Text& newInf) {
        btn = newBtn;
        inf = newInf;
    }
};

class PauseButton : public BUTTON {
public:
    PauseButton(RTangle BTN, Text INF) : BUTTON(BTN, INF) {}
    controls evaluate(controls BS) override {
        if (IsButtonClicked(GetMousePosition())) {
            if (BS.pause) {
                BS.pause = false;
                btn.setColor(DARKGRAY);
            } else {
                BS.pause = true;
                btn.setColor(DARKBLUE);
            }
        }
        Draw();
        return BS;
    }
};

class DirrchangeButton : public BUTTON {
public:
    DirrchangeButton(RTangle BTN, Text INF) : BUTTON(BTN, INF) {}
    controls evaluate(controls BS) override {
        if (IsButtonClicked(GetMousePosition())) {
            btn.setColor(ORANGE);
            if (BS.dir) {
                inf.setTxt("FWD");
                BS.dir = false;
            } else {
                inf.setTxt("RVS");
                BS.dir = true;
            }
        }
        Draw();
        btn.setColor(RED);
        return BS;
    }
};

class FPSincButton : public BUTTON {
public:
    FPSincButton(RTangle BTN, Text INF) : BUTTON(BTN, INF) {}
    controls evaluate(controls BS) override {
        if (IsButtonClicked(GetMousePosition())) {
            btn.setColor(BLUE);
            BS.framerate += 2;
        }
        Draw();
        btn.setColor(ORANGE);
        return BS;
    }
};

class FPSdecButton : public BUTTON {
public:
    FPSdecButton(RTangle BTN, Text INF) : BUTTON(BTN, INF) {}
    controls evaluate(controls BS) override {
        if (IsButtonClicked(GetMousePosition()) && BS.framerate > 2) {
            btn.setColor(ORANGE);
            BS.framerate -= 2;
        }
        Draw();
        btn.setColor(BLUE);
        return BS;
    }
};

class WLKrButton : public BUTTON {
public:
    WLKrButton(RTangle BTN, Text INF) : BUTTON(BTN, INF) {}
    controls evaluate(controls BS) override {
        if (IsButtonClicked(GetMousePosition())) {
            btn.setColor(PURPLE);
            BS.walk = 1;
            BS.pause = true;
        }
        Draw();
        btn.setColor(GREEN);
        return BS;
    }
};

class WLKlButton : public BUTTON {
public:
    WLKlButton(RTangle BTN, Text INF) : BUTTON(BTN, INF) {}
    controls evaluate(controls BS) override {
        if (IsButtonClicked(GetMousePosition())) {
            btn.setColor(PURPLE);
            BS.walk = -1;
            BS.pause = true;
        }
        Draw();
        btn.setColor(GREEN);
        return BS;
    }
};

class SELButton : public BUTTON {
public:
    SELButton(RTangle BTN, Text INF) : BUTTON(BTN, INF) {}
    controls evaluate(controls BS) override {
        if (IsButtonClicked(GetMousePosition())) {
            if (!BS.selection) {
                btn.setColor(DARKPURPLE);
                BS.pause = true;
                BS.selection = true;
            } else {
                BS.pause = true;
                BS.selection = false;
                btn.setColor(PURPLE);
            }
        }
        Draw();
        return BS;
    }
};

class MAINGUI{
    private:
    PauseButton PS;
    DirrchangeButton CD;
    FPSincButton FI;
    FPSdecButton FD;
    WLKrButton WR;
    WLKlButton WL;
    SELButton SL;
    controls CTRL;
    public:

    MAINGUI(RTangle* defRT, Text* defTXT)
        : PS(defRT[0], defTXT[0]),
          CD(defRT[1], defTXT[1]),
          FI(defRT[2], defTXT[2]),
          FD(defRT[3], defTXT[3]),
          WR(defRT[4], defTXT[4]),
          WL(defRT[5], defTXT[5]),
          SL(defRT[6], defTXT[6]) {}

    controls getConrols(){return CTRL;}
    void setTrck(int k){CTRL.trck=k;}
    void setVall(int k){CTRL.vall=k;}
    void setPause(bool k){CTRL.pause = k;}
    void setWalk(int k){CTRL.walk =k;}

    void EVAL(){
        CTRL = PS.evaluate(CTRL);
        CTRL = CD.evaluate(CTRL);
        CTRL = FI.evaluate(CTRL);
        CTRL = FD.evaluate(CTRL);
        CTRL = WR.evaluate(CTRL);
        CTRL = WL.evaluate(CTRL);
        CTRL = SL.evaluate(CTRL);
    }
};


class BubSortTape{

typedef struct BSNode {
    int* val;
    int step;
    int Aelem;
    int Belem;
    Color ord;
    bool fin = false;
    BSNode* next;
    BSNode* prev;
}BSListn, * BSListnp;

    private:
    BSListnp BEG;
    BSListnp END;
    int size;

    public:

    BubSortTape(int* VAL, int SIZE ):size(SIZE){
        int temp;
        int finished = false;
        BSListnp curr = new BSNode;
        BEG = new BSNode;
        END = new BSNode;
        int stp = 0;
        BSListnp p = new BSNode;

        BEG->val = VAL;
        BEG->Aelem = -1;
        BEG->Belem = -1;
        BEG->next = curr;
        BEG->prev = NULL;
        BEG->step = 0;
        BEG->ord = YELLOW;
        BEG->fin = false;
        curr = BEG;

        while(!finished){
            finished=true;
            for(int i=0;i<SIZE-1;i++){
                stp++;
                int* CVAL = new int[SIZE];
                for(int i = 0;i<size;i++){
                        CVAL[i]=VAL[i];
                    }
                    p = new BSNode;
                    curr->next = p;
                    p->prev = curr;
                    p->step=stp;
                    p->val = CVAL;
                    p->Aelem=i;
                    p->Belem=i+1;
                    p->fin = finished;
                    p->ord = YELLOW;
                    curr = p;

                if(VAL[i]>VAL[i+1]){
                    stp++;
                    temp = VAL[i];
                    VAL[i]=VAL[i+1];
                    VAL[i+1] = temp;
                    finished = false;

                    int* CVAL = new int[SIZE];
                    for(int i = 0;i<size;i++){
                        CVAL[i]=VAL[i];
                    }
                    p = new BSNode;
                    curr->next = p;
                    p->prev = curr;
                    p->step=stp;
                    p->val = CVAL;
                    p->Aelem=i;
                    p->Belem=i+1;
                    p->fin = finished;
                    p->ord = RED;
                    curr = p;
                }
                else{
                    p = new BSNode;
                    curr->next = p;
                    p->prev = curr;
                    p->step=stp;
                    p->val = CVAL;
                    p->Aelem=i;
                    p->Belem=i+1;
                    p->fin = finished;
                    p->ord = GREEN;
                    curr = p;
                }
            }
        }
        END = curr;
        END->next = NULL;
    }

    void Rend(BSListnp A, int trck){
        BSListnp crnt = A;
        char aux[10];
        if(A){
        if(crnt->fin)DrawRectangle(50,600,200,100,GREEN);
        else DrawRectangle(50,600,200,100,RED);
        DrawText("Value ", 20, 500, 30, BLACK);
        DrawText("Index: ", 20, 540, 30, BLACK);
        
        for(int i=0;i<size;i++){
            sprintf(aux, "%d",crnt->val[i]);
            DrawText(aux,205+i*40,500,25,BLACK);
            sprintf(aux, "%d",i);
            DrawText(aux,205+i*40,535,25,BLACK);
            if(i == crnt->Aelem || i== crnt->Belem){
                DrawRectangle(200+i*40,0,35,crnt->val[i]*3,crnt->ord);
            }
            else{if(crnt->val[i] == trck){
                DrawRectangle(200+i*40,0,35,crnt->val[i]*3, PURPLE);
                sprintf(aux, "%d",crnt->val[i]);
            DrawText(aux,205+i*40,500,25,PURPLE);
            sprintf(aux, "%d",i);
            DrawText(aux,205+i*40,535,25,PURPLE);
            }
            else DrawRectangle(200+i*40,0,35,crnt->val[i]*3,BLUE);
            }
            
        }
        }
    }

    void Display(){
        BSListnp crnt = BEG;
        MAINGUI BS(DEFRT, DEFTXT);
        int k;
        
        InitWindow(1600,900,"Bubble sort demo");
        SetTargetFPS(30);

        while (!WindowShouldClose()) {
        SetTargetFPS(BS.getConrols().framerate);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        Rend(crnt, BS.getConrols().vall);
        
        BS.EVAL();

        if(BS.getConrols().selection){
        if(IsKeyPressedRepeat(KEY_W))BS.setTrck(k=BS.getConrols().trck+1);
        if(IsKeyPressedRepeat(KEY_S))BS.setTrck(k=BS.getConrols().trck-1);
        BS.setVall(crnt->val[BS.getConrols().trck]);
        }

        if(!BS.getConrols().pause){
            if(BS.getConrols().dir){
                if(crnt->next)crnt=crnt->next;
                else BS.setPause(true);
            }
            else{
                if(crnt->prev)crnt=crnt->prev;
                else BS.setPause(true);

            }
        }
        switch (BS.getConrols().walk){
            case 1:
            if(crnt->next)crnt=crnt->next;
            break;
            case -1:
            if(crnt->prev)crnt=crnt->prev;
            break;
            default:
            break;
        }
        BS.setWalk(0);
        EndDrawing();
        }
    }
};

int main() {
    int sz = 30;
    int* A = new int[sz];
    for(int i=0;i<sz;i++){
        A[i]=rand()%100;
    }
    BubSortTape C(A, sz);
    C.Display();

    return 0;
}