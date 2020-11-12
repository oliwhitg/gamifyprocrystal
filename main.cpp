#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <random>
#include <string>
#include <sstream>
#include <chrono>

#include <sys/stat.h>

using namespace std;
using namespace sf;

//const int N = 6;
//int ts = 54; //tile size
//Vector2f offset(65,55);

const int N = 16;
int ts = 54; //tile size
Vector2f offset(65,55);

Vector2i Up(0,-1);
Vector2i Down(0,1);
Vector2i Right(1,0);
Vector2i Left(-1,0);
Vector2i DIR[4] = {Up,Right,Down,Left};

struct pipe{
    std::vector<Vector2i> dirs; //directions for a node
    int orientation; //orientation of nodes
    float angle; bool on;

    pipe() {angle=0;}

    void rotate(){
        for(int i=0;i<dirs.size();i++){
            if (dirs[i]==Up)  dirs[i]=Right;
            else if (dirs[i]==Right) dirs[i]=Down;
            else if (dirs[i]==Down)  dirs[i]=Left;
            else if (dirs[i]==Left)  dirs[i]=Up;
        }
    }
    bool isConnect(Vector2i dir){
        for(auto d: dirs){
            if (d==dir) return true;
        }
        return false;
    }
    void bendorstraighten() {
        if (dirs.size() == 2) {
            std::string s = "";
            for (auto d:DIR) s += isConnect(d) ? '1' : '0';

            if (s == "1010") {
                cout << "       LINEAR1" << endl;
                for (int i = 0; i < 2; i++) {
                    if (dirs[i] == Up) {
                        dirs[i] = Right;
                        orientation = 3;
                    }
                }
            } else if (s == "0101") {
                cout << "       LINEAR2" << endl;
                for (int i = 0; i < 2; i++) {
                    if (dirs[i] == Right) {
                        dirs[i] = Down;
                        orientation = 0;
                    }
                }
            } else if (s == "1100") {
                cout << "       Bent1" << endl;
                for (int i = 0; i < 2; i++) {
                    if (dirs[i] == Up) {
                        dirs[i] = Left;
                        orientation = 0;
                    }
                }
            } else if (s == "0110") {
                cout << "       Bent2" << endl;
                for (int i = 0; i < 2; i++) {
                    if (dirs[i] == Right) {
                        dirs[i] = Up;
                        orientation = 1;
                    }
                }
            } else if (s == "0011") {
                cout << "       Bent3" << endl;
                for (int i = 0; i < 2; i++) {
                    if (dirs[i] == Down) {
                        dirs[i] = Right;
                        orientation = 0;
                    }
                }
            } else if (s == "1001") {
                cout << "       Bent4" << endl;
                for (int i = 0; i < 2; i++) {
                    if (dirs[i] == Left) {
                        dirs[i] = Down;
                        orientation = 1;
                    }
                }
            }

        } else cout << "CANNOT FLIP THIS SHAPE" << endl;
    }
};

pipe grid[N][N];
pipe& cell(Vector2i v) {return grid[v.x][v.y];}
bool isOut(Vector2i v) {return !IntRect(0,0,N,N).contains(v);}

Vector2i putin(Vector2i neighbourpos) {
    //potential neighbour position, given vy v+d
    if (neighbourpos.x<0) neighbourpos.x+=(N);
    else if (neighbourpos.x>N-1) neighbourpos.x -=(N);
    if (neighbourpos.y<0) neighbourpos.y+=(N);
    else if (neighbourpos.y>N-1) neighbourpos.y-=(N);
    return neighbourpos;
}

bool checkneighbourdistance(Vector2i v, Vector2i d){
    Vector2i neighbourpos = putin(v+d);
    cout << neighbourpos.x - v.x <<","<<neighbourpos.y - v.y<< endl;
    if (int(abs(neighbourpos.x - v.x))==1 or int(abs(neighbourpos.x - v.x))==N-1){
        if (int(abs(neighbourpos.y - v.y))==1 or int(abs(neighbourpos.y - v.y))==N-1){
            return true;
        }
        else return false;
    }
    else return false;
}

int energynode(Vector2i v) {
    int ev = 0;
    for(auto d:DIR) {
        if (cell(v).isConnect(d)) {
            Vector2i neighbourpos = putin(v+d);
            if (!cell(neighbourpos).isConnect(-d)) {
                ev +=1;
            }
        }
    }
    return ev;
}

int vocalenergynode(Vector2i v) {
    int ev = 0;
    cout << "Selected Position : " << v.x << "," << v.y << endl;
    for(auto d:DIR) {
        if (cell(v).isConnect(d)) {
            Vector2i neighbourpos = putin(v+d);
            cout << "   Neighbour : " << neighbourpos.x <<","<< neighbourpos.y << endl;
//            cout << "   (" << neighbourpos.x-v.x << "," << neighbourpos.y-v.y<<")" << endl;
            cout << "      ("<<d.x<<","<<d.y<<")" << endl;
            for (int i=0;i<cell(neighbourpos).dirs.size();i++){
                cout << "       ("<<cell(neighbourpos).dirs[i].x<<","<<cell(neighbourpos).dirs[i].y <<")";
            }
            cout << endl;
            if (!cell(neighbourpos).isConnect(-d)) {
                cout << "       not connected" << endl;
                ev +=1;
            }
            else cout << "      connected" << endl;
        }
    }
    return ev;
}

int energytot(){
    int e = 0;
    for (int i = 0; i < N * N; i++) {
        int x = int(i % N);
        int y = (int((i - x) / N));
        e += energynode({x,y});
    }
    return e;
}

//
//void generatePuzzle(string filename){
//    //import node coordinations
//
//
//    cout <<"CHECK FILENAME : " << filename << endl;
//    ifstream secondinputFile(filename, ios::in);
//    if(!secondinputFile.good()) cout << "Cannot find input file procrystal.inpt" << endl;
//    string skip,line;
//    int numberlines=0;
//    int newfourcoordcount = 0;
//    int newthreecoordcount = 0;
//    int newtwocoordcount = 0;
//    int dummy;
//    while (getline(secondinputFile, line)){
//        ++numberlines;
//        istringstream(line)>>dummy;
//        dummy = int(dummy);
//        if (dummy == 2){
//            newtwocoordcount += 1;
//        }
//        else if (dummy == 4){
//            newfourcoordcount += 1;
//        }
//        else if (dummy == 30 or dummy == 31 or dummy ==3){
//            newthreecoordcount += 1;
//        }
//        else{
//            cout << "UNKNOWN VALUE" << endl;
//        }
//    }
//    cout << "               N+1 :  " << newfourcoordcount  << endl;
//    cout << "               N   :  " << newthreecoordcount << endl;
//    cout << "               N-2 :  " << newtwocoordcount   << endl;
//    cout << "               --------------------------" << endl;
//    cout << "               TOT :  " << newfourcoordcount+newthreecoordcount+newtwocoordcount << endl;
//    secondinputFile.close();
//
//    if (N*N != numberlines){
//        cout << "INPUT FILE HAS DIFFERENT DIMENSIONS TO EXPECTED" << endl;
//        exit(5);
//    }
//
//    cout << endl;
//    cout << "CHECKING THE RATIOS" << endl;
//
//    std::vector<Vector2i> nodes;
//
//
//    ifstream generateinputFile(checkfilename, ios::in);
//    numberlines = 0;
//    while (getline(generateinputFile, line)) {
//        istringstream(line) >> dummy;
//        dummy = int(dummy);
//        int x = int ((numberlines) % N);
//        int y = (int((numberlines - x) / N));
//        nodes.push_back(Vector2i(x,y));
//        pipe &p = grid[x][y];
//
//
//        int vecsize = p.dirs.size();
//        for (int i = 0; i < dummy; i++) {
//            if (i < vecsize) {
//                p.dirs[i] = DIR[i];
//            } else {
//                p.dirs.push_back(DIR[i]);
//            }
//        }
//        numberlines++;
//    }
////    {
////        int n = rand()%nodes.size();
////        Vector2i v = nodes[n];
////        Vector2i d = DIR[rand()%4];
////
////        if (cell(v).dirs.size()==3) {nodes.erase(nodes.begin() + n); continue;}
////        if (cell(v).dirs.size()==2) if (rand()%50) continue;
//////        if (cell(v).dirs.size()==2) break;
////
////
////        bool complete=1;
////        for(auto D:DIR){
////            if (!isOut(v+D) && cell(v+D).dirs.empty()) complete=0;
////        }
////        if (complete) {nodes.erase(nodes.begin() + n); continue; }
////
////        if (isOut(v+d)) continue;
////        if (!cell(v+d).dirs.empty()) continue;
////        cell(v).dirs.push_back(d);
////        cell(v+d).dirs.push_back(-d);
////        nodes.push_back(v+d);
////    }
//}
//


//void drop(Vector2i v> 6
//{
//   if (cell(v).on) return;
//   cell(v).on=true;
//
//   for(auto d:DIR)
//    if (!isOut(v+d))
//     if (cell(v).isConnect(d) && cell(v+d).isConnect(-d))
//       drop(v+d);
//}


int main(int argc, char **argv) {

    ifstream inputFile("./input.inpt", ios::in);
    if(!inputFile.good()) cout << "CANNOT READ INPUT FILE" << endl;
    string skip, line;

    getline(inputFile,line);
    getline(inputFile,line);

    float t1, t2, t3;
    int steps, gap;

    getline(inputFile,line);
    istringstream(line)>>t1;
    cout << "T1 : " << t1 << endl;

    getline(inputFile,line);
    istringstream(line)>>t2;
    cout << "T2 : " << t2 << endl;

    getline(inputFile,line);
    istringstream(line)>>t3;
    cout << "T3 : " << t3 << endl;

    getline(inputFile, line);

    getline(inputFile,line);
    istringstream(line)>>steps;
    cout << "Max steps : " << steps << endl;

    getline(inputFile,line);
    istringstream(line)>>gap;
    cout << "Step printout : " << gap << endl;

    const char *checkfilename;
    string filename;
    if (argc==2){
        cout << argv[1] << endl;
        const char *arg = argv[1];
        stringstream strValue;
        strValue << arg;
        int intValue;
        strValue >> intValue;
        filename = "./to_run_fixed_list_"+to_string(intValue);
        cout << filename << endl;
        filename = filename+".dat";
        cout << filename << endl;
        checkfilename = filename.c_str();
        ifstream f(checkfilename);
        bool fileexists = f.good();
        if (!fileexists) {
            cout << "NO SUCH FILE" << endl;
            exit(3);
        }
    }
    else if (argc==1){
        cout << "Using sample " << to_string(15) << endl;
        checkfilename = "./to_run_fixed_list_14.dat";
        filename = "./to_run_fixed_list_14.dat";
    }
    else{
        cout << "WRONG NUMBER OF ARGUMENTS" << endl;
        exit(3);
    }

    RenderWindow app(VideoMode(2.5*390, 2.5*390), "The Pipe Puzzle!");


    Texture t4,t5;
    t4.loadFromFile("./images/pipes.png");
    t4.setSmooth(true);
    t5.loadFromFile("./images/unpipes.png");
    t5.setSmooth(true);

    Sprite sPipe(t4);
    sPipe.setOrigin(27,27);
    Sprite sUnpipe(t5);
    sUnpipe.setOrigin(27,27);

    cout <<"CHECK FILENAME : " << checkfilename << endl;
    ifstream secondinputFile(checkfilename, ios::in);
    if(!secondinputFile.good()) cout << "Cannot find input file procrystal.inpt" << endl;
//    std::string skip,line;
    int numberlines=0;
    int newfourcoordcount = 0;
    int newthreecoordcount = 0;
    int newtwocoordcount = 0;
    int dummy;
    while (getline(secondinputFile, line)){
        ++numberlines;
        istringstream(line)>>dummy;
        dummy = int(dummy);
        if (dummy == 2){
            newtwocoordcount += 1;
        }
        else if (dummy == 4){
            newfourcoordcount += 1;
        }
        else if (dummy == 30 or dummy == 31 or dummy ==3){
            newthreecoordcount += 1;
        }
        else{
            cout << "UNKNOWN VALUE" << endl;
        }
    }
    cout << "               N+1 :  " << newfourcoordcount  << endl;
    cout << "               N   :  " << newthreecoordcount << endl;
    cout << "               N-2 :  " << newtwocoordcount   << endl;
    cout << "               --------------------------" << endl;
    cout << "               TOT :  " << newfourcoordcount+newthreecoordcount+newtwocoordcount << endl;
    secondinputFile.close();

    if (N*N != numberlines){
        cout << "INPUT FILE HAS DIFFERENT DIMENSIONS TO EXPECTED" << endl;
        exit(5);
    }

    cout << endl;
    cout << "CHECKING THE RATIOS" << endl;

    std::vector<Vector2i> nodes;


    ifstream generateinputFile(checkfilename, ios::in);
    numberlines = 0;
    while (getline(generateinputFile, line)) {
        istringstream(line) >> dummy;
        dummy = int(dummy);
        int x = int ((numberlines) % N);
        int y = (int((numberlines - x) / N));
        nodes.push_back(Vector2i(x,y));
        pipe &p = grid[x][y];


        int vecsize = p.dirs.size();
        for (int i = 0; i < dummy; i++) {
            if (i < vecsize) {
                p.dirs[i] = DIR[i];
            } else {
                p.dirs.push_back(DIR[i]);
            }
        }
        numberlines++;
    }

//    ifstream maininputFile("./to_run_fixed_list_2.dat", ios::in);
//    int numberlines = 0;
//    string skip,line;
//    int dummy;
//
//
//
//    while (getline(maininputFile, line)) {
//        istringstream(line) >> dummy;
//        dummy = int(dummy);
//        int x = (numberlines) % N;
//        int y = N-(int((numberlines - x) / N));
//
//        pipe &p = grid[x][y];
//
//        //orientation defined by davids to be added here
//
//        int vecsize = p.dirs.size();
//        std::string s = "";
//
//        for (auto d:DIR) s+= p.isConnect(d) ? '1' : '0';
//        if (s == "0011" || s == "0111" || s == "0101" || s == "1111") {
//            p.orientation = 0;
//            p.orientation = p.orientation%4;
//            p.rotate();
//        }
//        cout << numberlines << "  " << dummy << "  " << s << endl;
////        for (int n = 0; n < rand() % 4; n++) //shuffle//
////        {
////            grid[x][y].orientation++;
////            grid[x][y].rotate();
////        }
//        numberlines++;
//    }


    for(int i=0;i<N;i++) {
        for (int j = 0; j < N; j++) {

            pipe &p = grid[j][i];

            for (int n = 4; n > 0; n--) //find orientation//
            {
                std::string s = "";
                for (auto d: DIR) s += p.isConnect(d) ? '1' : '0';
//                cout << i << "," << j << "    " << s << endl;
                if (s == "0011" || s == "0111" || s == "0101" || s == "1111") p.orientation = n;
                p.rotate();
            }
        }
    }


    while (app.isOpen())
    {
        Event e{};
        Vector2i selectednode = {0,11};
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed) app.close();

            if (e.type == Event::MouseButtonPressed){
                if (e.key.code == Mouse::Left) {
                    Vector2i pos = Mouse::getPosition(app) + Vector2i(ts / 2, ts / 2) - Vector2i(offset);
                    pos /= ts;
                    if (isOut(pos)) continue;
                    cell(pos).orientation++;
                    cell(pos).orientation = cell(pos).orientation%4;
                    cell(pos).rotate();

                    selectednode = pos;

                    for (int i = 0; i < N; i++) {
                        for (int j = 0; j < N; j++) {
                            grid[j][i].on = 0;
                        }
                    }

                    pipe &p = grid[pos.x][pos.y];
                    //p.rotate();

                    for (int i = 0; i < N * N; i++) {
                        int x = int(i % N);
                        int y = (int((i - x) / N));

                        pipe &p = grid[x][y];
                        std::string s = "";
                        if (x == pos.x && y== pos.y) {
                            for (auto d:DIR) s += p.isConnect(d) ? '1' : '0';
                            cout << "String : " << s << endl;
                            cout << "Orient : " << p.orientation << endl;
                            cout << "Node E : " << energynode({x,y}) << endl;
                        }
//                        for (int j = 0; j < originalsize; j++) {
////                            cout << j << "  :  " << energy << endl;
//                            int x_trial;
//                            int y_trial;
//                            ////
//                            if (p.dirs[j].x < 0) {
//                                x_trial = x0;
//                                y_trial = y;
//
////                                cout << x_trial <<"," << y_trial << endl;
//                                pipe &q = grid[x_trial][y_trial];
//                                bool connect = false;
//                                int newsize = q.dirs.size();
//                                for (int k = 0; k < newsize; k++) {
//                                    if (q.dirs[k] == Right) connect = true;
//                                }
//
//                                if (x == pos.x && y== pos.y) cout << "LEFT : " << connect << endl;
//                                if (connect == false) energy += 1; // not true! need unsated cnxs
//                            } else if (p.dirs[j].x > 0) {
//                                x_trial = x1;
//                                y_trial = y;
////                                cout << x_trial <<"," << y_trial << endl;
//                                pipe &q = grid[x_trial][y_trial];
//                                bool connect = false;
//                                int newsize = q.dirs.size();
//                                for (int k = 0; k < newsize; k++) {
//                                    if (q.dirs[k] == Left) connect = true;
//                                }
//                                if (x == pos.x && y== pos.y) cout << "RIGHT : " << connect << endl;
//                                if (connect == false) energy += 1;
//                            } else {
//                                x_trial = x;
//                                if (p.dirs[j].y < 0) {
//                                    y_trial = y0;
////                                    cout << x_trial <<"," << y_trial << endl;
//                                    pipe &q = grid[x_trial][y_trial];
//                                    bool connect = false;
//                                    int newsize = q.dirs.size();
//                                    for (int k = 0; k < newsize; k++) {
//                                        if (q.dirs[k] == Down) connect = true;
//                                    }
//                                    if (x == pos.x && y== pos.y) cout << "TOP : " << connect << endl;
//                                    if (connect == false) energy += 1;
//                                } else if (p.dirs[j].y > 0) {
//                                    y_trial = y1;
////                                    cout << x_trial <<"," << y_trial << endl;
//                                    pipe &q = grid[x_trial][y_trial];
//                                    bool connect = false;
//                                    int newsize = q.dirs.size();
//                                    for (int k = 0; k < newsize; k++) {
//                                        if (q.dirs[k] == Up) connect = true;
//                                    }
//                                    if (x == pos.x && y== pos.y) cout << "BOTTOM : " << connect << endl;
//                                    if (connect == false) energy += 1;
//                                }
//                            }
//
//                        }
//
                    }
                    cout << "Energy : " << energytot() << endl;
                    cout << endl;
                }

                if (e.key.code == Mouse::Right){
                    Vector2i pos = Mouse::getPosition(app) + Vector2i(ts / 2, ts / 2) - Vector2i(offset);
                    pos /= ts;
                    if (isOut(pos)) continue;

                    cell(pos).bendorstraighten();

                }
//////
            }

            if (e.type == Event::KeyPressed){
                if (e.key.code == sf::Keyboard::Tab){
                    ofstream cnxsFile("gameoutput_"+to_string(energytot())+".dat");
                    if (cnxsFile.is_open()){
                        for(int i=0; i<N*N; ++i) {
                            int x = i%N;
                            int y = int((i-x)/N);
                            for (int j=0;j<cell({x,y}).dirs.size();j++){
                                int x0 = x+cell({x,y}).dirs[j].x;
                                int y0 = y+cell({x,y}).dirs[j].y;
                                //cout <<"    ("<<x0<<","<<y0<<")" << endl;
                                if (x0<0) x0+=(N);
                                else if (x0>N-1) x0-=(N);
                                if (y0<0) y0+=(N);
                                else if (y0>N-1) y0-=(N);
                                int ref = x0 + N*y0;
                                cout << "    "<<i<<"    "<<ref<<endl;
                                cnxsFile<<to_string(i)+"    "+to_string(ref)+"\n";
                            }
                        }

                    }

                }
            }

            if (e.type == Event::KeyPressed){
                if (e.key.code == sf::Keyboard::Enter){
                    //begin an mc round of 1000 steps
                    float temp;
                    int step = 0;
                    int initialenergy = energytot();
                    while (energytot()>0.5*initialenergy && step < steps){
//                    while (energytot()>0){

                        int mcnode = rand()%(N*N);
                        int numberrotations = rand()%3;

                        int x = mcnode%N;
                        int y = int((mcnode-x)/N);
                        Vector2i neighbour1 = putin({x+1,y});
                        Vector2i neighbour2 = putin({x-1, y});
                        Vector2i neighbour3 = putin({x,y+1});
                        Vector2i neighbour4 = putin({x,y-1});
                        bool run = true;
                        if (energynode({x,y})!=0){
                            run= true;
                        }
                        else if (energynode(neighbour1)!=0 or energynode(neighbour2)!=0 or energynode(neighbour3)!=0 or energynode(neighbour4)!=0) {
                            run=true;
                        }

//                        cout << x << "," << y << endl;
                        if (run) {

                            if (step % gap == 0) cout << "     STEP : " << step << "      " << energytot () << " / " << initialenergy << endl;
                            step++;
                            Vector2i pos = {x, y};
                            int e0 = energytot();

                            for (int j = 0; j < numberrotations + 1; j++) {
                                cell(pos).orientation++;
                                cell(pos).orientation = cell(pos).orientation % 4;
                                cell(pos).rotate();
                            }

                            int e1 = energytot();

                            if (e1 > e0) {
                                float rando = ((double) rand() / (RAND_MAX));
                                if (e0<6&&e0>4) temp=t2;
                                else if (e0<=4) temp=t3;
                                else temp=t1;
//                                cout << "RANDO : " << rando << "  vs : " << exp((e0-e1)/temp)<< endl;
                                if (temp == 0) {
                                    for (int j = 0; j < (3 - numberrotations); j++) {
                                        cell(pos).orientation++;
                                        cell(pos).rotate();
                                    }
                                } else if (rando > exp((e0 - e1) / temp)) {
                                    for (int j = 0; j < (3 - numberrotations); j++) {
                                        cell(pos).orientation++;
                                        cell(pos).rotate();
                                    }

                                }
                            }
                        }
                    }

                    int newfourcoordcount = 0;
                    int newthreecoordcount = 0;
                    int newtwocoordcount = 0;
                    int dummy;
                    for (int i=0;i<N*N;i++){
                        int x = i%N;
                        int y = int((i-x)/N);
                        dummy = int(cell({x,y}).dirs.size());
                        if (dummy == 2){
                            newtwocoordcount += 1;
                        }
                        else if (dummy == 4){
                            newfourcoordcount += 1;
                        }
                        else if (dummy == 30 or dummy == 31 or dummy ==3){
                            newthreecoordcount += 1;
                        }
                        else{
                            cout << "UNKNOWN VALUE" << endl;
                        }
                    }
                    cout << "               N+1 :  " << newfourcoordcount  << endl;
                    cout << "               N   :  " << newthreecoordcount << endl;
                    cout << "               N-2 :  " << newtwocoordcount   << endl;
                    cout << "               --------------------------" << endl;
                    cout << "               TOT :  " << newfourcoordcount+newthreecoordcount+newtwocoordcount << endl;


                    cout << "ENERGY  :  " << energytot() << endl;
                }
            }
        }

        app.clear();
        //app.draw(sBackground);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        int x;
        int y;
        for (int i=0;i<N*N;i++) {
            x = ((i) % N);
            y = (int((i - x) / N));
            //0,12
//        for (int i=0;i<2;i++){
//            if (i==1){
//                x = selectednode.x;
//                y = selectednode.y;
//            }
//            else {
//                pipe &q = grid[selectednode.x][selectednode.y];
//                x = selectednode.x + q.dirs[i].x;
//                y = selectednode.y + q.dirs[i].y;
//                if (x<0) x+=N;
//                else if (x>N) x-=N;
//                if (y<0) y+=N;
//                else if (y>N) y-=N;
//            }
            pipe &p = grid[x][y];

            p.angle += 5;
            if (p.angle > p.orientation * 90) p.angle = p.orientation * 90;

            int dummy = p.dirs.size();

            std::string s = "";
            for (auto d:DIR) s += p.isConnect(d) ? '1' : '0';
                if (energynode({x, y}) == 0) {
                    //                cout << "line 523" << endl;

                    if (dummy == 2) {
                        if (s == "1010" or s == "0101") sPipe.setTextureRect(IntRect(0, 0, ts, ts));
                        else sPipe.setTextureRect(IntRect(2 * ts, 0, ts, ts));
                    } else if (dummy == 3) sPipe.setTextureRect(IntRect(3 * ts, 0, ts, ts));
                    else if (dummy == 4) sPipe.setTextureRect(IntRect(ts, 0, ts, ts));
                    //                cout << "line 531" << endl;

                    sPipe.setRotation(p.angle);
                    sPipe.setPosition(x * ts, y * ts);
                    sPipe.move(offset);
                    //                cout << "line 536" << endl;
                    app.draw(sPipe);
                } else {
                    if (dummy == 2) {
                        if (s == "1010" or s == "0101") sUnpipe.setTextureRect(IntRect(0, 0, ts, ts));
                        else sUnpipe.setTextureRect(IntRect(2 * ts, 0, ts, ts));
                    } else if (dummy == 3) sUnpipe.setTextureRect(IntRect(3 * ts, 0, ts, ts));
                    else if (dummy == 4) sUnpipe.setTextureRect(IntRect(ts, 0, ts, ts));

                    sUnpipe.setRotation(p.angle);
                    sUnpipe.setPosition(x * ts, y * ts);
                    sUnpipe.move(offset);
                    app.draw(sUnpipe);
                }


        }
        app.display();




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//        for(int i=0;i<N;i++) {
//            for (int j = 0; j < N; j++) {
//                pipe &p = grid[j][i];
//                int kind = p.dirs.size();
////                p.dirs = {};
//                cout << i<<","<<j<<"    "<<p.coordinaitionno() - kind << endl;
//                if (kind == 2 && p.dirs[0] == -p.dirs[1]) kind = 0;
//                if (kind == 1) kind = 3;
//                p.angle += 5;
//                if (p.angle > p.orientation * 90) p.angle = p.orientation * 90;
//
//                sPipe.setTextureRect(IntRect(ts * kind, 0, ts, ts));
//                sPipe.setRotation(p.angle);
//                sPipe.setPosition(j * ts, i * ts);
//                sPipe.move(offset);
//                app.draw(sPipe);
//
////                 if (kind==1)
////                     { if (p.on) sComp.setTextureRect(IntRect(53,0,36,36));
////                       else sComp.setTextureRect(IntRect(0,0,36,36));
////                       sComp.setPosition(j*ts,i*ts);sComp.move(offset);
////                       app.draw(sComp);
////                     }
//            }
//        }
//
////        app.draw(sServer);
//        app.display();
    }

    return 0;
}
