from flask import Flask, jsonify, request
import requests
from datetime import datetime, date,timedelta
from calendar import monthcalendar
import json
import os

app = Flask(__name__)

# API URL for retrieving UK bank holidays
GOV_UK_URL = "https://www.gov.uk/bank-holidays.json"

# Load Chinese holiday data
file_path = os.path.join(app.root_path, 'chineseholidy.json')
with open(file_path, 'r') as f:
    chinese_holidays = json.load(f)

file_path = os.path.join(app.root_path,'ddl.json')
with open(file_path, 'r') as f:
    ddl = json.load(f)

def get_term_start(year):
    """
    Get the term start date: 5th week in September
    """
    september = monthcalendar(year, 9)
    fifth_week = september[4]
    term_start = date(year, 9, fifth_week[0] if fifth_week[0] != 0 else fifth_week[1]) + timedelta(weeks=1)
    return term_start

def calculate_week(term_start, current_date):
    """
    Adjust logic to accurately calculate which week of the term the current date falls into.
    """
    days_difference = (current_date - term_start).days # Calculate the difference in days
    if days_difference < 0:  # If the current date is before the term start date
        return "The current date is before the term start date"
    current_week = int(days_difference / 7) + 1
    return current_week

def determine_which_term(current_week):
    """
    Determine which term the current week belongs to.
    """
    if current_week <= 15:
        return 1
    elif current_week <= 30:
        return 2
    else:
        return 3

def get_left_term_weeks(term_order, current_week):
    """
    Calculate the number of weeks left in the current term.
    """
    if term_order == 1:
        return 16 - current_week
    elif term_order == 2:
        return 31 - current_week
    elif term_order == 3:
        return 38 - current_week
    return 0

@app.route('/term-week', methods=['GET'])
def get_term_week():
    """
    API endpoint to get the current term week.
    """
    try:
        current_date = date.today()
        current_date = date(2024,12,20)
        current_year = current_date.year

        # Determine the academic year
        if current_date >= date(current_year, 9, 1):
            term_start = get_term_start(current_year)
        else:
            term_start = get_term_start(current_year - 1)

        week_number = calculate_week(term_start, current_date)
        if isinstance(week_number, str):  # If week_number is an error message
            return jsonify({
                "current_date": current_date.isoformat(),
                "message": week_number
            }), 400

        term_order = determine_which_term(week_number)
        left_weeks = get_left_term_weeks(term_order, week_number)

        return jsonify({
            "term_start": term_start.isoformat(),
            "current_date": current_date.isoformat(),
            "week_number": week_number,
            "term": term_order,
            "left_weeks": left_weeks
        })

    except Exception as e:
        return jsonify({"error": str(e)}), 500

@app.route('/next-holiday', methods=['GET'])
def get_next_holiday():
    """
    API endpoint to get the next UK bank holiday.
    """
    try:
        response = requests.get(GOV_UK_URL)
        data = response.json()
        holidays = data["england-and-wales"]["events"]
        current_date = date.today()

        for holiday in holidays:
            holiday_date = datetime.strptime(holiday["date"], "%Y-%m-%d").date()
            if holiday_date > current_date:
                days_until = (holiday_date - current_date).days
                return jsonify({
                    "name": holiday["title"],
                    "date": holiday["date"],
                    "days_until": days_until
                })

        return jsonify({"error": "No upcoming holidays found"}), 404

    except Exception as e:
        return jsonify({"error": str(e)}), 500

@app.route('/next-chinese-holiday', methods=['GET'])
def get_next_chinese_holiday():
    """
    API endpoint to get the next Chinese holiday.
    """
    try:
        current_date = datetime.now().date()

        # Find the next upcoming holiday in Chinese holidays
        for holiday_date, holiday_details in sorted(chinese_holidays.items()):
            holiday_date_obj = datetime.strptime(holiday_date, "%Y-%m-%d").date()
            if holiday_date_obj > current_date:
                days_until = (holiday_date_obj - current_date).days
                return jsonify({
                    "name": holiday_details["name"],
                    "date": holiday_details["date"],
                    "days_until": days_until
                })

        return jsonify({"error": "No upcoming Chinese holidays found"}), 404

    except Exception as e:
        return jsonify({"error": str(e)}), 500


@app.route('/ddl', methods=['GET'])
def get_courses():
    # Get the current date and time
    current_time = datetime.now()

    # List to store courses with non-expired deadlines
    filtered_courses = []
    for course in ddl["courses"]:
        # List to store non-expired deadlines for the current course
        filtered_deadlines = []
        for deadline in course["deadlines"]:
            # Combine the date and time fields into a datetime object
            deadline_time = datetime.strptime(f"{deadline['date']} {deadline['time']}", "%Y-%m-%d %H:%M")
            # Check if the deadline is in the future
            if deadline_time > current_time:
                filtered_deadlines.append(deadline)
        # Only include the course if it has at least one non-expired deadline
        if filtered_deadlines:
            filtered_courses.append({
                "name": course["name"],
                "code": course["code"],
                "deadlines": filtered_deadlines
            })

    # Return the filtered courses as a JSON response
    return jsonify({"courses": filtered_courses})


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
