use std::io;
use std::io::prelude::*;

// Helpers ///////////////////////////////////////////////////////////////////
fn read() -> Vec<i32> {
    io::stdin()
        .lock()
        .lines()
        .map(|line| line.unwrap().trim().parse().unwrap())
        .collect()
}

// Solutions /////////////////////////////////////////////////////////////////
fn solve_a(data: &Vec<i32>) -> i32 {
    let mut acc: i32 = 0;
    for i in 1..data.len() {
        if data[i] > data[i - 1] {
            acc += 1;
        }
    }
    acc
}

// This works, but I think in a language with imperative structures
// the first way is simpler and clearer
fn solve_a_func(data: &Vec<i32>) -> i32 {
    data.iter().fold(vec![0, 1000000], |acc, x| {
        if x > &acc[1] {
            vec![acc[0] + 1, *x]
        } else {
            vec![acc[0], *x]
        }
    })[0]
}

fn solve_b(data: &Vec<i32>, w: usize) -> i32 {
    let mut last: i32 = 0;
    for i in 0..w {
        last += data[i];
    }

    let mut acc: i32 = 0;
    for i in w..data.len() {
        let cur = (last + data[i]) - data[i - w];
        if cur > last {
            acc += 1;
        }
        last = cur;
    }
    acc
}

// Main //////////////////////////////////////////////////////////////////////
fn main() {
    let data = read();
    //println!("{:?}", data);

    println!("{}", solve_a(&data));
    println!("{}", solve_b(&data, 3));

    //println!("{}", solve_a_func(&data));
}
