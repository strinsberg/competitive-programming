use std::convert::TryInto;
use std::io;
use std::io::prelude::*;

// Helpers ///////////////////////////////////////////////////////////////////
fn read() -> Vec<Vec<u32>> {
    io::stdin()
        .lock()
        .lines()
        .map(|line| {
            line.unwrap()
                .chars()
                .map(|ch| ch.to_digit(10).unwrap())
                .collect()
        })
        .collect()
}

// If least then it will find the least common binary digit, otherwise the most.
// For ties when looking for most common it will return 1 and for least 0.
fn common_digit(data: &Vec<Vec<u32>>, j: usize, least: u32) -> u32 {
    let mut count: u32 = 0;
    for i in 0..data.len() {
        count += data[i][j];
    }

    let n: u32 = data.len().try_into().unwrap();
    if count * 2 < n {
        0 ^ least
    } else {
        1 ^ least
    }
}

// Convert a vector of binary digits to an unsigned decimal number
fn digits_to_dec(digits: &Vec<u32>) -> u32 {
    let s = digits.iter().map(|x| x.to_string()).collect::<String>();
    u32::from_str_radix(&s, 2).unwrap()
}

fn calc_power(data: &Vec<Vec<u32>>, epsilon: u32) -> u32 {
    let m = data[0].len();
    let mut digits = vec![0; m];
    for j in 0..m {
        digits[j] = common_digit(data, j, epsilon);
    }
    digits_to_dec(&digits)
}

fn calc_life(data: &Vec<Vec<u32>>, co2: u32) -> u32 {
    let mut res = data.clone();
    for j in 0..res[0].len() {
        if res.len() == 1 {
            break;
        }
        let cd = common_digit(&res, j, co2);
        res.retain(|xs| xs[j] == cd);
    }

    digits_to_dec(&res[0])
}

// Solutions /////////////////////////////////////////////////////////////////
fn solve_a(data: &Vec<Vec<u32>>) -> u32 {
    let gamma: u32 = calc_power(data, 0);
    let epsilon: u32 = calc_power(data, 1);
    gamma * epsilon
}

fn solve_b(data: &Vec<Vec<u32>>) -> u32 {
    let oxygen: u32 = calc_life(data, 0);
    let co2: u32 = calc_life(data, 1);
    oxygen * co2
}

// Main //////////////////////////////////////////////////////////////////////
fn main() {
    let data = read();
    //println!("{:?}", data);

    println!("{}", solve_a(&data));
    println!("{}", solve_b(&data));
}
