import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
import requests
import datetime
from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session["user_id"]
    data = db.execute("SELECT * FROM portfolio WHERE user_id = ?", user_id)
    cash = db.execute("SELECT cash FROM users WHERE id=?", user_id)[0]["cash"]
    stocks = db.execute("SELECT stock_name, num_shares FROM portfolio WHERE user_id = ?", user_id)
    prices = {}
    total_portfolio_value = cash

    # create dict prices that maps stock = shares
    # tally up total portfolio value
    for stock in stocks:
        prices[stock['stock_name']] = lookup(stock['stock_name'])['price']
        total_portfolio_value += prices[stock['stock_name']] * stock['num_shares']

    return render_template("index.html", data=data, cash=cash, stocks=stocks, prices=prices, total_portfolio_value=total_portfolio_value)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == 'GET':
        return render_template("buy.html")
    else:
        query = lookup(request.form.get("symbol"))
        symbol = query["symbol"]
        shares = int(request.form.get("shares"))

        if not symbol or symbol is None:
            return apology("that symbol does not exist", 403)

        if int(shares) <= 0:
            return apology("number of shares must be a positive number.", 403)

        user_id = session["user_id"]
        cash = db.execute("SELECT cash FROM users WHERE id=?", user_id)[0]["cash"]
        cost =  int(query["price"]) * shares

        if cost > cash:
            return apology("you do not have enough money to complete the purchase", 403)

        # user has enough cash to make the transaction, update their total remaining cash
        remaining_cash = cash - cost
        time_stamp = datetime.datetime.now()
        # update our users and transactions table
        db.execute("INSERT INTO transactions (user_id, stock_name, num_shares, action, date, price) VALUES (?, ?, ?, ?, ?, ?)", (user_id, symbol, shares, "BUY", time_stamp, cost))
        db.execute("UPDATE users SET cash=? WHERE id=?", remaining_cash, user_id)

        # if already in portfolio
        if db.execute("SELECT num_shares FROM portfolio WHERE user_id = ? AND stock_name = ?", user_id, symbol):
            db.execute("UPDATE portfolio SET num_shares = num_shares + ? WHERE stock_name = ? AND user_id = ?", shares, symbol, user_id)
        # if not already in portfolio
        else:
            db.execute("INSERT INTO portfolio(user_id, stock_name, num_shares) VALUES(?, ?, ?)", user_id, symbol, shares)
        return redirect("/")


@app.route("/deposit", methods=["GET", "POST"])
@login_required
def deposit():
    """Deposit Funds"""
    if request.method == 'GET':
        return render_template("deposit.html")
    else:
        user_id = session["user_id"]
        deposit_amount = int(request.form.get("amount"))
        password = request.form.get("password")
        rows = db.execute("SELECT * FROM users WHERE id = :id",
                          id=user_id)
        time_stamp = datetime.datetime.now()
        if not password:
            return apology("must provide password", 403)

        # Ensure old password is correct
        elif not check_password_hash(rows[0]["hash"], password):
            return apology("Sorry, the current password is incorrect!", 403)

        elif deposit_amount <= 0:
            return apology("You must enter an amount greater than 0!", 403)

        cash = db.execute("SELECT cash FROM users WHERE id=?", user_id)[0]["cash"] + deposit_amount
        db.execute("INSERT INTO transactions (user_id, stock_name, num_shares, action, date, price) VALUES (?, ?, ?, ?, ?, ?)", (user_id, "-", "-", "DEPOSIT", time_stamp, deposit_amount))
        db.execute("UPDATE users SET cash=? WHERE id=?", cash, user_id)
        return redirect("/")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    data = db.execute("SELECT * FROM transactions WHERE user_id = ?", user_id)
    return render_template("history.html", data=data)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == 'GET':
        return render_template("quote.html")
    else:
        symbol = lookup(request.form.get("symbol"))
        return render_template("quoted.html", stock_data=symbol)

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == 'GET':
        return render_template("register.html")
    else:
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        username_check = db.execute("SELECT username FROM users WHERE username = :username", username)

        # Ensure username was submitted
        if not username:
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not password:
            return apology("must provide password", 403)

        # Ensure password confirmation was submitted
        elif not confirmation:
            return apology("must provide password confirmation", 403)

        # Ensure password matches confirmation
        elif not (password == confirmation):
            return apology("passwords must match!", 403)

        # Ensure username not already in database
        elif len(username_check) == 1:
            return apology("username already exists", 403)

        # Ok to insert into database
        else:
            db.execute("INSERT INTO users (username, hash, cash) VALUES (:username, :hash, :cash)", username, generate_password_hash(password), 10000)
            return render_template("login.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session["user_id"]
    stocks = db.execute("SELECT stock_name FROM portfolio WHERE user_id = ?", user_id)

    if request.method == 'GET':
        return render_template("sell.html", stocks=stocks)
    else:
        query = lookup(request.form.get("symbol"))
        symbol = query["symbol"]
        num_shares_sold = int(request.form.get("shares"))
        shares = db.execute("SELECT num_shares FROM portfolio WHERE user_id = ? AND stock_name = ?", user_id, symbol)[0]['num_shares']

        if num_shares_sold == 0:
            return apology("you must sell more than 0 shares.", 403)

        if shares < num_shares_sold:
            return apology("you do not own that amount of shares.", 403)

        cash = db.execute("SELECT cash FROM users WHERE id=?", user_id)[0]["cash"]
        cost =  int(query["price"]) * num_shares_sold

        # update cash amount with value of shares sold
        updated_cash_amount = cash + cost
        time_stamp = datetime.datetime.now()

        # update our users and transactions table
        db.execute("INSERT INTO transactions (user_id, stock_name, num_shares, action, date, price) VALUES (?, ?, ?, ?, ?, ?)", (user_id, symbol, num_shares_sold, "SELL", time_stamp, cost))
        db.execute("UPDATE users SET cash=? WHERE id=?", updated_cash_amount, user_id)

        # if selling all stocks
        if shares - num_shares_sold == 0:
            db.execute("DELETE FROM portfolio WHERE stock_name = ? AND user_id = ?", symbol, user_id)
        else:
            db.execute("UPDATE portfolio SET num_shares = num_shares - ? WHERE stock_name = ? AND user_id = ?", num_shares_sold, symbol, user_id)

        return redirect("/")

@app.route("/update", methods=["GET", "POST"])
@login_required
def update():
    """Update user password"""
    if request.method == 'GET':
        return render_template("update.html")
    else:
        user_id = session["user_id"]
        old_password = request.form.get("old_password")
        new_password = request.form.get("new_password")
        confirmation = request.form.get("confirmation")

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE id = :id",
                          id=user_id)

        # Ensure current password was submitted
        if not old_password:
            return apology("must provide current password", 403)

        # Ensure new password was submitted
        elif not new_password:
            return apology("must provide new password", 403)

        # Ensure password confirmation was submitted
        elif not confirmation:
            return apology("must provide current password", 403)

        # Ensure old password is correct
        elif not check_password_hash(rows[0]["hash"], old_password):
            return apology("Sorry, the current password is incorrect!", 403)

        # Ensure password matches confirmation
        elif not (new_password == confirmation):
            return apology("passwords must match!", 403)

        db.execute("UPDATE users SET hash = ? WHERE id = ?", generate_password_hash(new_password), user_id)
        return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
