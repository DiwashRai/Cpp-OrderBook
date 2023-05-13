# Cpp-OrderBook

Orderbook/Matching engine written in cpp to learn more about matching engines and design with low latency performance in mind.

My initial aim was just to implement a simple matching engine for fun.
However, I discovered an old link to a contest ([Quantcup 1](https://sites.google.com/site/quantcup/)) held to implement the fastest matching engine.
I then decided to implement a matching engine without any specialized knowledge or techniques and compare with the winning implementation.

My first implementation ended up being twice as slow as the very best submission for the competition.
This is still able to go through 37,000+ orders in 0.25s whilst handling submitting limit orders as well as cancelling.

## Demo

![](https://github.com/DiwashRai/Cpp-OrderBook/blob/main/assets/OrderExecutions-Console-Output.gif)

This is the matching engine set to output order executions to the console. Bear in mind that the engine works much faster than is shown in the gif.
The engine is bottlenecked by the speed of the console output in the gif.

## Performance

The score that the winning quantcup implementation gets on my machine (ryzen 3600 cpu) is the following:
```
mean(latency) = 160.45, sd(latency) = 81.30
You scored 120.88. Try to minimize this.
```
The score that my second implementation gets on the same machine is the following:
```
mean(latency) = 175.27, sd(latency) = 127.64
You scored 151.46. Try to minimize this.
```

## Notes/Summary

My first implementation performed a lot better than expected considering I had no prior knowledge regarding matching engine implementation.

From this experience, I have learnt that the most common operations for an orderbook actually seem to be adds and cancellations.
Executions and cancellations are both O(1), however, add is only O(logN). I can improve this by adding a hash table for existing price levels.
This means that a price level can be found in O(1) time instead of having to traverse the buy/sell tree.

