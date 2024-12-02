from datetime import date
from calendar import monthcalendar

def get_term_start(year):
    """
    Get the term start date: 5th week in September
    """
    september = monthcalendar(year, 9)  
    fifth_week = september[4] 
    term_start = date(year, 9, fifth_week[0] if fifth_week[0] != 0 else fifth_week[1])
    return term_start

def calculate_week(term_start, current_date):
    """
    Adjust logic to accurately calculate which week of the term the current date falls into.
    """
    days_difference = (current_date - term_start).days  # Calculate the difference in days
    if days_difference < 0:  # If the current date is before the term start date
        return "The current date is before the term start date"

    # Start counting from Week 2 after a full 7 days
    if days_difference % 7 == 0:  # If the difference is exactly a multiple of 7 days
        current_week = days_difference // 7
    else:  # If there are leftover days (not a full week)
        current_week = days_difference // 7 + 1
    return current_week

def determine_which_term(currentWeek):
    term_order = 0
    if currentWeek <= 15 :
        term_order = 1
    elif currentWeek > 15 and currentWeek <= 30 :
        term_order = 2
    elif currentWeek > 30:
        term_order = 3
    
    return term_order

def determine_current_term_week():
    """
    Automatically determine the current week of the term based on today's date.
    """
    current_date = date.today()  # Get today's date
    current_year = current_date.year

    # Determine whether the current date belongs to the current academic year or the previous one
    if current_date >= date(current_year, 9, 1):  # If today is on or after September 1
        term_start = get_term_start(current_year)  # Use this year's term start date
    else:  # If today is before September 1, it belongs to the previous academic year's term
        term_start = get_term_start(current_year - 1)

    week_number = calculate_week(term_start, current_date)  # Calculate the week number
    return term_start, current_date, week_number

# Get the result
term_start, current_date, week_number = determine_current_term_week()

# Output the results
if isinstance(week_number, int):
    print(f"The term started on {term_start}. Today, {current_date}, is term {determine_which_term(week_number)} Week {week_number}.")
else:
    print(f"Today, {current_date}: {week_number}")

