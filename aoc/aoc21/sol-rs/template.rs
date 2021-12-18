use std::io;
use std::io::prelude::*;

// Helpers ///////////////////////////////////////////////////////////////////
fn read() -> Vec<String> {
    io::stdin()
        .lock()
        .lines()
        .map(|line| line.unwrap())
        .collect()
}

// Solutions /////////////////////////////////////////////////////////////////
//fn solve_a(data: &Vec<Move>) -> i64 {}

//fn solve_b(data: &Vec<Move>) -> i64 {}

// Main //////////////////////////////////////////////////////////////////////
fn main() {
    let data = read();
    println!("{:?}", data);

    //println!("{}", solve_a(&data));
    //println!("{}", solve_b(&data));
}
