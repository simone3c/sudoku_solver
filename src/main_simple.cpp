#include <array>
#include <cstdint>
#include <algorithm>
#include <print>
#include <chrono>
#include <ranges>
#include <unordered_map>
#include <utility>
#include <cassert>

struct sudoku_t{  
    std::array<uint8_t, 81> sud;
    std::string input;

    static constexpr auto is_valid_entry = [](uint8_t n) -> bool {
        return n > 0 && n < 10;
    };

    constexpr bool row_contains(uint8_t elem, uint8_t row) const {
        for(int i = 0; i < 9; ++i){
            if((*this)[row, i] == elem){
                return true;
            } 
        }
        return false;
    }

    constexpr bool col_contains(uint8_t elem, uint8_t col) const {
        for(int i = 0; i < 9; ++i){
            if((*this)[i, col] == elem){
                return true;
            } 
        }
        return false;
    }

    constexpr bool square_contains(uint8_t elem, uint8_t n) const {

        uint8_t i, j;
        switch (n){
        case 0:
            i = 0;
            j = 0;
            break;
        case 1:
            i = 0;
            j = 3;
            break;
        case 2:
            i = 0;
            j = 6;
            break;
        case 3:
            i = 3;
            j = 0;
            break;
        case 4:
            i = 3;
            j = 3;
            break;
        case 5:
            i = 3;
            j = 6;
            break;
        case 6:
            i = 6;
            j = 0;
            break;
        case 7:
            i = 6;
            j = 3;
            break;
        case 8:
            i = 6;
            j = 6;
            break;
        default:
            std::unreachable();
        }

        for(int a = 0; a < 3; ++a){
            for(int b = 0; b < 3; ++b){
                if((*this)[i + a, j + b] == elem){
                    return true;
                } 
            }
        }
        
        return false;
    }

    constexpr bool check_row(uint8_t row) const {
        for(int i = 0; i < 9; ++i){
            if(!is_valid_entry((*this)[row, i])){
                return false;
            }
        }
        return true;
    }

    constexpr bool check_col(uint8_t col) const {
        for(int i = 0; i < 9; ++i){
            if(!is_valid_entry((*this)[i, col])){
                return false;
            }
        }
        return true;
    }

    constexpr bool check_square(uint8_t n) const {
        for(int i = 0; i < 9; ++i){
            if(!is_valid_entry((*this)[3 * (n/3) + i / 3, 3 * (n %3) + (i%3)])){
                return false;
            }
        }
        return true;
    }

    constexpr bool solve() {
        _solve(0);

        for(int i = 0; i < 9; ++i){
            if(!check_row(i) || !check_col(i) || !check_square(i)){
                return false;
            }
        }
        return true;
    }

    constexpr bool _solve(uint8_t p){

        std::chrono::_V2::system_clock::time_point start;
        std::chrono::_V2::system_clock::time_point end;

        while(p < 81 && sud[p] != 0){
            ++p;
        }

        if(p < 81){
            const uint8_t i = p / 9;
            const uint8_t j = p % 9;
            const uint8_t square = 3 * (i / 3) + (j / 3);

            for(int e = 1; e < 10; ++e){

                if(row_contains(e, i) || 
                   col_contains(e, j) || 
                   square_contains(e, square)
                ){
                    continue;
                }

                (*this)[i, j] = e;
    
                if(_solve(p + 1)){
                    return true;
                }

            }
                
            (*this)[i, j] = 0;
            return false;
        }

        return true;
    }

    constexpr uint8_t& operator[](uint8_t row, uint8_t col){
        return sud[(row << 3) + row + col];
    }

    constexpr const uint8_t& operator[](uint8_t row, uint8_t col) const {
        return sud[(row << 3) + row + col];
    }

    constexpr sudoku_t(std::string i): input(i){
        reset();
    }

    constexpr void reset(){
        for(auto [pos, c] : std::views::zip(sud, input)){
            pos = c - '0';
        }
    }
};

int main(int argc, char** argv){

    if(argc != 3){
        std::println("Usage: ./main.out <sudoku> <repetitions>");
        return 1;
    }

    sudoku_t s(argv[1]);

    uint64_t rep = std::stoull(argv[2]);
    uint64_t total = 0;

    assert(s.solve());

    for(int i = 0; i < rep; ++i){
        s.reset();
        auto start = std::chrono::high_resolution_clock().now();
        s.solve();
        auto end = std::chrono::high_resolution_clock().now();
        total += std::chrono::duration<uint64_t, std::nano>(end - start).count();
    }

    // for(auto& [k, v] : map)
    //     std::println("{} -> {} ns", k, v);
    
    std::println("{}", total / rep);
}