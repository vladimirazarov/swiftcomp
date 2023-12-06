var result : Int
var n : Int = 3
if (n < 2) {
result = 1
} else {
let decremented_n = decrement(of: n, by: 1)
let temp_result = factorial(decremented_n)
result = n * temp_result
}
write(result)
