#include <array>
#include <cstdint>
#include <algorithm>
#include <print>
#include <chrono>
#include <ranges>
#include <optional>

struct sudoku_t{
    std::array<uint8_t, 81> sud;
    std::string input;

    static constexpr auto is_valid_entry = [](uint8_t n) -> bool {
        return n > 0 && n < 10;
    };

    constexpr auto get_row(uint8_t n) const {
        return sud | std::views::drop(9 * n) | std::views::take(9);
    }

    constexpr auto get_col(uint8_t n) const {
        return sud | std::views::drop(n) | std::views::stride(9);
    }

    constexpr auto get_square(uint8_t n) const {
        return sud | 
            std::views::drop(27 * (n / 3) + 3 * (n % 3)) |
            std::views::chunk(3) |
            std::views::stride(3) |
            std::views::take(3) |
            std::views::join;
    }

    constexpr bool row_contains(uint8_t elem, uint8_t row) const {
        return std::ranges::count(get_row(row), elem);
    }

    constexpr bool col_contains(uint8_t elem, uint8_t col) const {
        return std::ranges::count(get_col(col), elem) > 0;
    }

    constexpr bool square_contains(uint8_t elem, uint8_t square) const {
        return std::ranges::count(get_square(square), elem) > 0;
    }

    constexpr bool check_row(uint8_t row) const {
        return std::ranges::all_of(get_row(row), is_valid_entry);
    }

    constexpr bool check_col(uint8_t col) const {
        return std::ranges::all_of(get_col(col), is_valid_entry);
    }

    constexpr bool check_square(uint8_t square) const {
        return std::ranges::all_of(get_square(square), is_valid_entry);
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
        return sud[row * 9 + col];
    }

    constexpr const uint8_t& operator[](uint8_t row, uint8_t col) const {
        return sud[row * 9 + col];
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
#include <cassert>
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
    
    std::println("{}", total / rep);
}