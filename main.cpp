#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <utility>
#include <string>
#include <algorithm>
#include <ranges>
#include <functional>

class adjacency_list
{
    std::vector< std::string > empty ;
    std::map< std::string, std::vector< std::string > > graph ;
public :
    void add( std::string const & a, std::string const & b )
    {
        graph[a].push_back(b) ;
    }
    void add( std::string && a, std::string && b )
    {
        graph[std::move(a)].push_back(std::move(b)) ;
    }

    // f( node, node_list )
    void for_each( auto && f ) const
    {
        std::ranges::for_each( graph, [&]( auto const & pair )
        {
            auto const & [ node, node_list ] = pair ;
            f( node, node_list ) ;
        } ) ;
    }

    auto const & get( std::string const & key ) const
    {
        auto iter = graph.find(key) ;
        if ( iter == graph.end() )
            return empty ;
        else
            return iter->second ;
    }

    void print() const
    {
        for_each( []( auto const & node, auto const & node_list )
        {
            std::cout << node << "\n    " ;
            std::ranges::for_each( node_list, []( auto const & y )
            {
                std::cout << y << ' ' ;
            }) ;
            std::cout << '\n' ;
        }) ;
    }
} ;

enum struct COLOR { WHITE=0, GRAY, BLACK } ;


auto topological_sort( adjacency_list const & adj )
{
    std::stack< std::string > stack ;
    std::map< std::string, COLOR > color ;
    std::function<void( std::string const & key, std::vector<std::string> const &)> visit = [&]( auto const & key, auto const & node_list ) 
    {
        auto c = color[key] ;
        if ( c == COLOR::BLACK )
            return ;

        if ( c == COLOR::GRAY )
        {
            std::cerr << "error: cycle is detected for " << key << '\n'  ;
            std::exit(1) ;
        }

        color[key] = COLOR::GRAY ;
        std::ranges::for_each( node_list, [&](auto const & key)
        {
            visit( key, adj.get(key) ) ;
        }) ;
        color[key] = COLOR::BLACK ;
        stack.push(key) ;
    } ;

    adj.for_each([&]( auto const & key, auto const & node_list )
    {
        if ( color[key] != COLOR::BLACK )
            visit( key, node_list ) ;
    } ) ;

    return stack ;
}

int main()
{
    adjacency_list adj ;
    std::string a, b ;

    while ( std::cin >> a >> b )
    {
        adj.add( std::move(a), std::move(b) ) ;
    }

    auto stack = topological_sort( adj )  ;

    while ( stack.empty() == false )
    {
        std::cout << stack.top() << '\n' ;
        stack.pop() ;
    }
}
