use std::println;

fn main() {
    let x = 20;
    println!("x is {x}");
    let mut sum = 0;
    for i in 0..x {
        println!("{i}");
        sum += i;
    }
    println!("{sum}");
}
