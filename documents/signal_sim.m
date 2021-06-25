sig = [600, 600, 600, 600, 600, 600, 30, 30, 30, 30, 30, 30, -700, -700, -700, -700, -700, -800, -800, -700, 500, 500, 500, 700]
plot(sig)
sig_ran = sig + 100 * (rand(1, 24) - 0.5)
plot(sig_ran)
h = [0.2, 0.8]
h1 = [0.05, 0.1, 0.15, 0.2, 0.25, 0.25]
y = conv(h, sig_ran)
y = y(2:end)
y1 = conv(h1, sig_ran)
y1 = y1(6:end)
hold on
plot(y)
plot(y1)
hold off
legend('signal', 'proposed', 'compare')