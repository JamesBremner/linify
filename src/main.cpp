#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <wex.h>
#include "cStarterGUI.h"

struct cLinify
{
    std::vector<cxy> myPoints;
    std::vector<std::pair<cxy, cxy>> myLines;

    cLinify(const std::vector<cxy> &points);

    void solve();
    void displayLines();

    bool isLine(
        const cxy &p1,
        const cxy &p2,
        const cxy &p3);

    std::vector<double> box();

    bool test();

    std::vector<cxy>& getPoints()
    {
        return myPoints;
    }
    std::vector<std::pair<cxy, cxy>> getLines()
    {
        return myLines;
    }
};

bool cLinify::isLine(
    const cxy &p1,
    const cxy &p2,
    const cxy &p3)
{
    const double COLLINEAR_TOLERANCE = 0.005;
    // triangle area method
    //(x2−x1)(y3−y1)=(x3−x1)(y2−y1)
    // https://math.stackexchange.com/questions/38338/methods-for-showing-three-points-in-mathbbr2-are-colinear-or-not/221530#221530

    return abs((p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y)) < COLLINEAR_TOLERANCE;
}
cLinify::cLinify(const std::vector<cxy> &points)
    : myPoints(points)
{
}

void cLinify::solve()
{

    cxy lp1, lp2;

    for (int k = 0; k < myPoints.size() - 2; k++)
    {
        if( k == 0 )
        {
            lp1 = myPoints[0];
            lp2 = myPoints[1];
            continue;
        }
        if (isLine(myPoints[k-1], myPoints[k], myPoints[k + 1]))
        {
            // extend line
            lp2 = myPoints[k+1];
        }
        else
        {
            // line ended
            myLines.push_back(std::make_pair(lp1, lp2));
            lp1 = lp2;
        }
    }
    myLines.back() = std::make_pair(lp1, myPoints.back());
}

void cLinify::displayLines()
{
    for (auto &l : myLines) {
        std::cout
            << "( " << l.first.x << ", " << l.first.y
            << " ) to ( " << l.second.x << ", " << l.second.y
            << " )\n";
    }
}
std::vector<double> cLinify::box()
{
    cxy topleft;
    topleft = myPoints[0];
    for (auto &p : myPoints)
    {
        if (p.x < topleft.x)
            topleft.x = p.x;
        if (p.y < topleft.y)
            topleft.y = p.y;
    }
    cxy wh(0, 0);
    for (auto &p : myPoints)
    {
        if (p.x - topleft.x > wh.x)
            wh.x = p.x - topleft.x;
        if (p.y - topleft.y > wh.y)
            wh.y = p.y - topleft.y;
    }
    std::vector<double> ret;
    double xscale = 400 / wh.x;
    double yscale = 400 / wh.y;
    if (yscale < xscale)
        ret.push_back(yscale);
    else
        ret.push_back(xscale);
    ret.push_back( topleft.x);
    ret.push_back( topleft.y);

    return ret;
}
bool cLinify::test()
{
    cLinify L({cxy(1, 1),
               cxy(2, 2),
               cxy(3, 3)});

    if (!L.isLine(cxy(1, 1), cxy(2, 2), cxy(3, 3)))
        return false;
    if (L.isLine(cxy(1, 1), cxy(2, 2), cxy(3, 3.1)))
        return false;
    std::cout << "unit test passed\n";
    return true;
}
class cGUI : public cStarterGUI
{
public:
    cGUI(cLinify &L)
        : cStarterGUI(
              "Linify",
              {50, 50, 1000, 500}),
          myLinify(L)
    {

        show();
        run();
    }

    virtual void draw(wex::shapes &S);

private:
    cLinify &myLinify;
};

void cGUI::draw(wex::shapes &S)
{
    auto thebox = myLinify.box();
    S.color(0);
    for( auto& p : myLinify.getPoints() )
    {
        int x = 10 + thebox[0] * ( p.x - thebox[1] );
        int y = 10 + thebox[0] * ( p.y - thebox[2] );
        S.circle( x, y, 5 );
    }
    S.color(0x0000FF);
    for( auto& l : myLinify.getLines())
    {
        std::vector<int> vl
        {
            10 + thebox[0] * ( l.first.x - thebox[1] ),
            10 + thebox[0] * ( l.first.y - thebox[2] ),
            10 + thebox[0] * ( l.second.x - thebox[1] ),
            10 + thebox[0] * ( l.second.y - thebox[2] )
        };
        S.line(vl);

    }

}

main()
{
    /*
    // Example input for testing

const table = [
    { Position: [0.849999487, -1.47224224] },
    { Position: [0.848479152, -1.01117814] },
    { Position: [0.842648506, -0.707066119] },
    { Position: [0.848704576, -0.489999771] },
    { Position: [0.845723033, -0.307818025] },
    { Position: [0.846934378, -0.149337426] },
    { Position: [0.859999716, 0] },
    { Position: [0.846934378, 0.149337381] },
    { Position: [0.845723033, 0.307817996] },
    { Position: [0.848704517, 0.489999801] },
    { Position: [0.842648506, 0.707066059] },
    { Position: [0.848479271, 1.01117814] },
    { Position: [0.599999666, 1.03923011] },
    { Position: [0.376222014, 1.03366148] },
    { Position: [0.184067041, 1.04389584] },
    { Position: [0, 1.0399996] },
    { Position: [-0.184066996, 1.04389584] },
    { Position: [-0.376221985, 1.03366148] },
    { Position: [-0.599999726, 1.03922999] },
    { Position: [-0.874190629, 1.04181993] },
    { Position: [-1.24099123, 1.04131532] }
];
*/
    cLinify L({cxy(0.849999487, -1.47224224),
               cxy(0.848479152, -1.01117814),
               cxy(0.842648506, -0.707066119),
               cxy(0.848704576, -0.489999771),
               cxy(0.845723033, -0.307818025),
               cxy(0.846934378, -0.149337426),
               cxy(0.859999716, 0),
               cxy(0.846934378, 0.149337381),
               cxy(0.845723033, 0.307817996),
               cxy(0.848704517, 0.489999801),
               cxy(0.842648506, 0.707066059),
               cxy(0.848479271, 1.01117814),
               cxy(0.599999666, 1.03923011),
               cxy(0.376222014, 1.03366148),
               cxy(0.184067041, 1.04389584),
               cxy(0, 1.0399996),
               cxy(-0.184066996, 1.04389584),
               cxy(-0.376221985, 1.03366148),
               cxy(-0.599999726, 1.03922999),
               cxy(-0.874190629, 1.04181993),
               cxy(-1.24099123, 1.04131532)});
    L.test();
    L.solve();
    L.displayLines();

    cGUI theGUI(L);
    return 0;
}
