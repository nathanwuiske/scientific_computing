import numpy
import matplotlib.pyplot as plt

def equations(currentSolution):
    x1 = currentSolution[0]
    x2 = currentSolution[1]
    x3 = currentSolution[2]
    return numpy.array([(x1*tunaDeathRate)-(tunaGrowthRate*x1*x2), (-x2*sharkDeathRate)+(sharkGrowthRate*x1*x2)-(fishingRate*x2*x3),
                        (-humanDeathRate*x3)+(humanGrowth*x2*x3)])

def timeStepping(prevSolution, function, timeIncrement):
    return prevSolution + timeIncrement * function(prevSolution)

# Set parameters
tunaDeathRate = 0.5
tunaGrowthRate = 1
sharkGrowthRate = 1
sharkDeathRate = 4
fishingRate = 2
humanDeathRate = 0.001
humanGrowth = 0.001
finalTime  = 20
timeIncrement = 0.01
steps  = int(finalTime/timeIncrement) + 1
tunaPopulation = 50
sharkPopulation = 12
humanPopulation = 5

# Main
totalLst = numpy.empty((steps, 3))
totalLst[0] = numpy.array([tunaPopulation, sharkPopulation, humanPopulation])
for i in range(steps-1):
    totalLst[i+1] = timeStepping(totalLst[i], equations, timeIncrement)

# Designation
time = numpy.linspace(0.0, finalTime, steps)
tunaLst = totalLst[:,0]
sharkLst = totalLst[:,1]
humanLst = totalLst[:,2]

# Plotting code
fig, ax = plt.subplots(figsize=(8,5))
plt.plot(time, tunaLst, label = 'Tuna', c='dodgerblue')
plt.plot(time, sharkLst, label = 'Shark', c='red')
plt.title("Predator-Prey Model")
plt.xlabel("Time")
plt.ylabel("Growth")
plt.legend()

#plot two
plt.plot(tunaLst, sharkLst, c='slateblue')
plt.title("Predator vs Prey population")
plt.ylabel("# of Sharks")
plt.xlabel("# of Tuna")