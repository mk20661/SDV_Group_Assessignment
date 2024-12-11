from flask import Flask, jsonify, request
import requests
from datetime import datetime, date
from calendar import monthcalendar

app = Flask(__name__)

# API URL for retrieving UK bank holidays
GOV_UK_URL = "https://www.gov.uk/bank-holidays.json"

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

    current_week = int(days_difference / 7)
    return current_week

def determine_which_term(current_week):
    """
    Determine which term the current week belongs to.
    """
    term_order = 0
    if current_week <= 15:
        term_order = 1
    elif current_week > 15 and current_week <= 30:
        term_order = 2
    elif current_week > 30:
        term_order = 3
    return term_order

def get_left_term_weeks(term_order, current_week):
    left_week = 0
    if term_order == 1 or term_order == 2:
        left_week = 15 - current_week + 1
    elif term_order == 3:
        left_week = 7 - current_week + 1
    
    return left_week


@app.route('/term-week', methods=['GET'])
def get_term_week():
    """
    API endpoint to get the current term week.
    """
    try:
        current_date = date.today()  # Get today's date
        current_year = current_date.year

        # Determine whether the current date belongs to the current academic year or the previous one
        if current_date >= date(current_year, 9, 1):  # If today is on or after September 1
            term_start = get_term_start(current_year)  # Use this year's term start date
        else:  # If today is before September 1, it belongs to the previous academic year's term
            term_start = get_term_start(current_year - 1)

        week_number = calculate_week(term_start, current_date)  # Calculate the week number
        term_order = determine_which_term(week_number)

        if isinstance(week_number, int):
            return jsonify({
                "term_start": term_start.isoformat(),
                "current_date": current_date.isoformat(),
                "week_number": week_number,
                "term": term_order,
                "left_weeks": get_left_term_weeks(term_order, week_number)
            })
        else:
            return jsonify({
                "current_date": current_date.isoformat(),
                "message": week_number
            }), 400

    except Exception as e:
        return jsonify({"error": str(e)}), 500

@app.route('/next-holiday', methods=['GET'])
def get_next_holiday():
    """
    API endpoint to get the next UK bank holiday.
    """
    try:
        # Fetch holiday data from the API
        response = requests.get(GOV_UK_URL)
        data = response.json()

        # Focus on "england-and-wales" region data
        holidays = data["england-and-wales"]["events"]

        # Get the current date and time
        current_date = datetime.now()

        # Find the next upcoming holiday
        next_holiday = None
        days_until = None  # To calculate the number of days until the holiday
        for holiday in holidays:
            holiday_date = datetime.strptime(holiday["date"], "%Y-%m-%d")
            if holiday_date > current_date:
                next_holiday = holiday
                days_until = (holiday_date - current_date).days
                break

        # If a next holiday is found, return its data and days until
        if next_holiday:
            return jsonify({
                "name": next_holiday["title"],
                "date": next_holiday["date"],
                "days_until": days_until
            })

        # If no future holidays are found, return an error message
        return jsonify({"error": "No upcoming holidays found"}), 404

    except Exception as e:
        # Handle any errors that occur during the process
        return jsonify({"error": str(e)}), 500

if __name__ == '__main__':
    # Run the Flask application on host 0.0.0.0 and port 5000
    app.run(host='0.0.0.0', port=5000)