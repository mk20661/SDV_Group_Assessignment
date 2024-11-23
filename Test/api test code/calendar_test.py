from datetime import date
from calendar import monthcalendar

def get_term_start(year):
    """
    获取学期开始日期：每年9月的第5个星期的星期一。
    """
    september = monthcalendar(year, 9)
    fifth_week = september[4]
    term_start = date(year, 9, fifth_week[0] if fifth_week[0] != 0 else fifth_week[1])
    return term_start

def calculate_week(term_start, current_date):
    """
    修正逻辑，精确计算当前日期是学期的第几周。
    """
    days_difference = (current_date - term_start).days
    if days_difference < 0:
        return "当前日期早于学期开始日期"

    # 满7天后从第2周开始计数
    if days_difference % 7 == 0:  # 恰好整周
        current_week = days_difference // 7
    else:  # 天数不足一整周
        current_week = days_difference // 7 + 1
    return current_week

def determine_current_term_week():
    """
    自动根据当前日期判断是第几周。
    """
    current_date = date.today()  # 获取当前日期
    current_year = current_date.year

    # 判断是否在当前学年或上一学年
    if current_date >= date(current_year, 9, 1):  # 9月1日之后
        term_start = get_term_start(current_year)
    else:  # 9月1日之前，属于上一学年的学期
        term_start = get_term_start(current_year - 1)

    week_number = calculate_week(term_start, current_date) - 1
    return term_start, current_date, week_number

# 获取结果
term_start, current_date, week_number = determine_current_term_week()

if isinstance(week_number, int):
    print(f"从 {term_start} 开始，今天 {current_date} 是第 {week_number} 周。")
else:
    print(f"今天 {current_date} ：{week_number}")
