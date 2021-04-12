# What is this, and why should I use it?

MealPlanner is project I've been working on-and-off for a few years. 

It's gone through many iterations, and started as a simple program to help my family plan meals ahead of time. This version is a complete rebuild from the ground-up, and is designed to be relatively small and portable as it runs on a Windows/UNIX terminal.

MealPlanner focuses on the finance and convenience aspect of planning meals in advance. That is, it saves you the time and headache creating shopping lists since you'll know well in advance what you need to buy. And it saves you money by sticking to a budget.

The program works with 3 fundamental types of objects that you create:

- Meals, which explains you what you're going to eat and how much it costs.

- Tags, customizable filters which you assign to one or more Meals.

- MultiTags, a collection of Tags, which allow you to define complex rules.

Meal Plans are saved to a .txt file and includes a summary of each week.

No matter how simple or complex your meal arrangements are, my rule system is designed to be as inclusive of various planning styles as possible.

If you're someone who doesn't worry about what to eat, MealPlanner can still benefit you! Simple rules are easy to create, and the budgeting aspect can save you money.

If you're the sole cook of a large household and would like some way to better plan daily meals in advance, then this is the program for you!

# Features

- Create Meal Plans for 1 week, up to an entire year!

- Randomly chooses Meals to schedule while respecting your set restrictions.

- Meal Plans adhere to your set budget.

- Track your spending by saving past Meal Plans.

- Can plan Meals for any number of people.

- Supports any dietary restrictions.

- Supports meals cooked in large batches.

# Limitations

- Does not track calories (yet!). I may add this feature if there's any demand for it.

- Cannot plan for specific calendar day/week/month. The program works in units of 7 days, and does not keep track of a real-world month. It would add significant complexity for what I see as a minor benefit. If you think I'm wrong, let me know!

- Program uses a terminal window to display interface. This is because I have little skills in creating GUIs, and the program is intended for personal use. MealPlanner is intended to be a small, portable tool to aid in the planning of meals.


# Screenshots
![mainmenu](https://github.com/NoorA0/MealPlanner/blob/main/screenshots/01_mainmenu.jpg)
![manual](https://github.com/NoorA0/MealPlanner/blob/main/screenshots/02_manual.jpg)
![mealmenu](https://github.com/NoorA0/MealPlanner/blob/main/screenshots/04_mealmenu.jpg)
![credits](https://github.com/NoorA0/MealPlanner/blob/main/screenshots/03_credits.jpg)

# Installation Instructions

## Windows

1. Download your appropriate release version, create a folder and place the executable file inside of it.

2. Run MealPlanner.exe 

## Mac/Linux

1. Download your appropriate release version, create a folder and place MealPlanner inside of it.

2. Run MealPlanner using a terminal. (On Linux: right click the folder and select "open terminal here", then type `./MealPlanner`)

As I don't have a Mac to test on, the instructions may differ slightly, but should be very similar.

# Manual

The program includes a detailed manual, which is accessible from the main menu.

# Troubleshooting

Q: Help! The program is displaying strangely.\
A: Resize the window! If the terminal is too small, the text won't display properly.


Example of the following issue:
![SmallWindow](https://github.com/NoorA0/MealPlanner/blob/main/screenshots/05_windowtoosmall.jpg)



Q: I think I've found a bug.\
A: Please contact me at noorga115@gmail.com and include: a description of the bug, your save file. You can also create an issue on Github.
