use std::io;
use std::io::prelude::*;

// A lot of this code is sort of unecessary, but it helps me get used to using
// some rust srtuctures. It also makes the code quite readable and I think
// is more how I would write it if I had to do something similar in real
// life rather than just finish a quick problem.

// Helpers ///////////////////////////////////////////////////////////////////
#[derive(Debug)]
enum Move {
    Forward(i64),
    Up(i64),
    Down(i64),
}

impl Move {
    fn new(dir: &str, v: i64) -> Move {
        match dir {
            "forward" => Self::Forward(v),
            "up" => Self::Up(v),
            "down" => Self::Down(v),
            _ => panic!(),
        }
    }
}

fn read() -> Vec<Move> {
    io::stdin()
        .lock()
        .lines()
        .map(|line| {
            line.unwrap()
                .split_whitespace()
                .map(|s| s.to_owned())
                .collect()
        })
        .map(|pair: Vec<String>| Move::new(&pair[0], pair[1].parse().unwrap()))
        .collect()
}

struct Pos {
    x: i64,
    y: i64,
    aim: i64,
}

// Solutions /////////////////////////////////////////////////////////////////
fn solve_a(data: &Vec<Move>) -> i64 {
    let p = data
        .iter()
        .fold(Pos { x: 0, y: 0, aim: 0 }, |acc, x| match x {
            Move::Forward(amt) => Pos {
                x: acc.x + amt,
                y: acc.y,
                aim: 0,
            },
            Move::Up(amt) => Pos {
                x: acc.x,
                y: acc.y - amt,
                aim: 0,
            },
            Move::Down(amt) => Pos {
                x: acc.x,
                y: acc.y + amt,
                aim: 0,
            },
        });
    p.x * p.y
}

fn solve_b(data: &Vec<Move>) -> i64 {
    let p = data
        .iter()
        .fold(Pos { x: 0, y: 0, aim: 0 }, |acc, x| match x {
            Move::Forward(amt) => Pos {
                x: acc.x + amt,
                y: acc.y + (acc.aim * amt),
                aim: acc.aim,
            },
            Move::Up(amt) => Pos {
                x: acc.x,
                y: acc.y,
                aim: acc.aim - amt,
            },
            Move::Down(amt) => Pos {
                x: acc.x,
                y: acc.y,
                aim: acc.aim + amt,
            },
        });
    p.x * p.y
}

// Main //////////////////////////////////////////////////////////////////////
fn main() {
    let data = read();
    //println!("{:?}", data);

    println!("{}", solve_a(&data));
    println!("{}", solve_b(&data));
}
