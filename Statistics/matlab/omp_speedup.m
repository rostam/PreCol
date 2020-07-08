ths = [1 2 3 4 5 6 7 8 9 10];
ser = 42.8745;
spd = [ser/42.8745 ser/33.9665 ser/25.2741 ser/20.6863 ser/21.4943 ...
       ser/20.1796 ser/17.9640 ser/16.1068 ser/15.4174 ser/16.1545];
   
ser1 = 96.795;
spd1 = [ser1/96.795 ser1/75.7442 ser1/55.6050 ser1/49.3350 ser1/49.3607...
       ser1/49.3650 ser1/45.342 ser1/43.254 ser1/40.742 ser1/39.416];


%plot(ths,spd,'-r*','LineWidth',2);
%xlabel('The number of threads','FontWeight','bold','FontSize',18);
%ylabel('Speedup','FontWeight','bold','FontSize',18);

plot(ths,spd1,'-r*','LineWidth',2);
xlabel('The number of threads','FontWeight','bold','FontSize',18);
ylabel('Speedup','FontWeight','bold','FontSize',18);
