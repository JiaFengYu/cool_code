use std::io;
use std::time::Instant;

// REWRITE THIS USING DYNAMIC PROGRAMMING

fn dynamic_fibonnaci(n:u32) -> u32 {
    let mut dp = vec![0; n as usize];
    dp[0] = 1;
    dp[1] = 1;

    for i in 2..n as usize{
        dp[i] = dp[i-1]+dp[i-2];
    }
    dp[(n-1) as usize]
}

fn recursive_fibonacci(n:u32) -> u32 {
    if n <= 1 {
        return n;
    }
    recursive_fibonacci(n-1) + recursive_fibonacci(n-2)
}



fn main() {
    println!("N'th Fibonacci number?");
    let mut input = String::new();
    io::stdin()
        .read_line(&mut input)
        .expect("Failed to read line");
    let input: u32 = input.trim().parse().expect("Please type a number!");
    let start_dp = Instant::now();
    println!("{}th Fibonacci number using bottom-up DP: {}", input, dynamic_fibonnaci(input));
    let duration_dp = start_dp.elapsed();
    println!("Time for DP impl: {:?}", duration_dp);
    let start = Instant::now();
    println!("{}th Fibonacci number using naive recursion: {}", input, recursive_fibonacci(input));
    let duration = start.elapsed();
    println!("Time for recursive impl: {:?}", duration);
}
