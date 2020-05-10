#include <optional>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <io2d.h>
#include "route_model.h"
#include "render.h"
#include "route_planner.h"

using namespace std::experimental;
/*std optional  value that may or may not bre present .the return value of a function that may fail it is converted to bool to infom use if there*/
/*is any object or not.*/
static std::optional<std::vector<std::byte>> ReadFile(const std::string &path)
{   
    std::ifstream is{path, std::ios::binary | std::ios::ate};/*I did an IFstream called is and i gave it the path of the file and the mode of operation*/
    /*binary means that the operation is perofmred in binary mode rather than text/ ate the output postion start at the end of the file.*/
    /*seek to the end of the inputstream*/
    if( !is )
        return std::nullopt;
    
    auto size = is.tellg();/*thia help us o know the size of the input stream*/
    std::vector<std::byte> contents(size);    /*make vector intialized to that size*/
    
    is.seekg(0);
    is.read((char*)contents.data(), size);/*read all the input stream into the content vector*/

    if( contents.empty() )
        return std::nullopt;
    return std::move(contents);/*this is used to tranfer content of this vector to another variable without using pointhers or refrences*/
}

int main(int argc, const char **argv)
{    
    std::string osm_data_file = "";
    if( argc > 1 ) {
        for( int i = 1; i < argc; ++i )
            /*-f allow you to specific the osm data you want to use*/
            if( std::string_view{argv[i]} == "-f" && ++i < argc )
                osm_data_file = argv[i];
    }
    else {
        std::cout << "To specify a map file use the following format: " << std::endl;
        std::cout << "Usage: [executable] [-f filename.osm]" << std::endl;
        osm_data_file = "../map.osm";
    }
    
    std::vector<std::byte> osm_data;/*vector of bytes*/
 
    if( osm_data.empty() && !osm_data_file.empty() ) {
        std::cout << "Reading OpenStreetMap data from the following file: " <<  osm_data_file << std::endl;
        auto data = ReadFile(osm_data_file);
        if( !data )
            std::cout << "Failed to read." << std::endl;
        else
            osm_data = std::move(*data);
    }
    
    // TODO 1: Declare floats `start_x`, `start_y`, `end_x`, and `end_y` and get
    // user input for these values using std::cin. Pass the user input to the
    // RoutePlanner object below in place of 10, 10, 90, 90.
    float start_x;
    float start_y;
    float end_x;
    float end_y;
    std::cin>>start_x>>start_y>>end_x>>end_y;

    // Build Model.
    RouteModel model{osm_data};
    // Create RoutePlanner object and perform A* search.
    RoutePlanner route_planner{model, start_x, start_y, end_x, end_y};
    route_planner.AStarSearch();

    std::cout << "Distance: " << route_planner.GetDistance() << " meters. \n";

    // Render results of search.
    Render render{model};

    auto display = io2d::output_surface{400, 400, io2d::format::argb32, io2d::scaling::none, io2d::refresh_style::fixed, 30};
    display.size_change_callback([](io2d::output_surface& surface){
        surface.dimensions(surface.display_dimensions());
    });
    display.draw_callback([&](io2d::output_surface& surface){
        render.Display(surface);
    });
    display.begin_show();
}
