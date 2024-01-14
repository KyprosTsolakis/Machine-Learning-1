#include <boost/test/unit_test.hpp>

#include <stdexcept>

#include "types.h"
#include "waypoints.h"
#include "analysis-route.h"
#include "gridworld-route.h"

using namespace GPS;


///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( Route_TotalLength )
//Position of each route point

const RoutePoint routept1 = { Position(30,50,100), "P1" };
const RoutePoint routept2 = { Position(50,100,200), "P2" };
const RoutePoint routept3 = { Position(40,-150,300), "P3" };
const RoutePoint routept4 = { Position(40,4,400), "P4" };
const RoutePoint routept5 = { Position(-90,-180,500), "P5" };
const RoutePoint routept6 = { Position(90,180,500), "P6" };
const RoutePoint routept7 = { Position(50,140,500), "P7" };
const RoutePoint routept8 = { Position(50,140,100), "P8" };
const RoutePoint routept9 = { Position(70,90,100), "P9" };


// Error cases: empty route points
BOOST_AUTO_TEST_CASE (EmptyRoute)
{
    const std::vector<RoutePoint> routePoints = {};
    const Analysis::Route route {routePoints};

    BOOST_CHECK_THROW (route.totalLength(), std::domain_error);
}

//Typical same latitude and longtitude, different elevation
BOOST_AUTO_TEST_CASE (CheckVerticalDifferens )
{
    const std::vector<RoutePoint> routePoints = {routept7,routept8};
    const Analysis::Route route {routePoints};
    const double expectedLength = 400;

    const double actualLength = route.totalLength();

    BOOST_CHECK_EQUAL (actualLength, expectedLength );
}

//Typical same elevation different latitudes and longtitude
BOOST_AUTO_TEST_CASE (CheckSameElevation)
{
    const std::vector<RoutePoint> routePoints = {routept8,routept9};
    const Analysis::Route route {routePoints};
    const double expectedLength = 3397324.6333631645 ;

    const double actualLength = route.totalLength();

    BOOST_CHECK_EQUAL (actualLength, expectedLength );
}

//Typical case: How actually the function works
BOOST_AUTO_TEST_CASE (DifferentPoints )
{
    const std::vector<RoutePoint> routePoints = {routept1,routept2,routept3};
    const Analysis::Route route {routePoints};
    const double expectedLength = 12597536.681634463;

    const double actualLength = route.totalLength();

    BOOST_CHECK_EQUAL (actualLength, expectedLength );
}

//Typical case: Duplicate Points
BOOST_AUTO_TEST_CASE (DuplicatedPoints)
{
    const std::vector<RoutePoint> routePoints = { routept1, routept2,routept1,routept4};
    const Analysis::Route route {routePoints};
    const double lengthSoFar = 13668913.677859593;

    const double actualLength = route.totalLength();

    BOOST_CHECK_EQUAL (actualLength, lengthSoFar );
}

//Atypical test: Testing only one point
BOOST_AUTO_TEST_CASE (onePoint)
{
    const std::vector<RoutePoint> routePoints = {routept1};
    const Analysis::Route route {routePoints};
    const double expectedLength = 0;

    const double actualLength = route.totalLength();

    BOOST_CHECK_EQUAL (actualLength, expectedLength );
}

//Edge Case: The firsts and the lasts possible values for latitude and longtitude
BOOST_AUTO_TEST_CASE (FirstLastValues)
{
    const std::vector<RoutePoint> routePoints = {routept5,routept6};
    const Analysis::Route route {routePoints};
    const double expectedLength = 20015114.442035925;

    const double actualLength = route.totalLength();

    BOOST_CHECK_EQUAL (actualLength, expectedLength );
}


BOOST_AUTO_TEST_SUITE_END()

///////////////////////////////////////////////////////////////////////////////
