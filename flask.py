
# A very simple Flask Hello World app for you to get started with...
from sqlalchemy.orm.attributes import flag_modified
from flask import Flask, redirect, render_template, request, jsonify, abort, flash
from flask_sqlalchemy import SQLAlchemy
#from wtforms import  IntegerField, TextAreaField, SubmitField, RadioField, SelectField
#from sqlalchemy import desc ,asc
from flask_marshmallow import Marshmallow
#from marshmallow import Schema, fields, pprint
#from datetime import datetime, timedelta
#import  os
#from os.path import isfile, join
#from os import listdir
#import json
#from io import StringIO
#from werkzeug.wrappers import Response
#import itertools
#import random
#import string

app = Flask(__name__)

SQLALCHEMY_DATABASE_URI = "mysql+mysqlconnector://{username}:{password}@{hostname}/{databasename}".format(
    username="ZbigniewK",
    password="alamakota", # database passowrd hidden
    hostname="ZbigniewK.mysql.pythonanywhere-services.com",
    databasename="ZbigniewK$Squash-Ball-Heater",
)
app.config["SQLALCHEMY_DATABASE_URI"] = SQLALCHEMY_DATABASE_URI
app.config["SQLALCHEMY_POOL_RECYCLE"] = 299 # connection timeouts
app.config["SQLALCHEMY_TRACK_MODIFICATIONS"] = False # no warning disruptions

db = SQLAlchemy(app)
ma = Marshmallow(app)

class Users(db.Model):

    __tablename__ = "User"
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.VARCHAR(255))


    def __init__(self, name):
        self.name = name

class UsersSchema(ma.Schema):
    class Meta:
        # Fields to expose
        fields = ('id' ,'name')

user_schema = UsersSchema()
users_schema = UsersSchema(many=True)


class Machine(db.Model):
    __tablename__ = "Machine"
    id = db.Column(db.Integer, primary_key=True)
    owner_id = db.Column(db.Integer)
    addres = db.Column(db.VARCHAR(255))
    set_temperature = db.Column(db.Float)
    status = db.Column(db.Boolean)


    def __init__(self, owner_id, addres, set_temperature, status):
        self.owner_id = owner_id
        self.addres = addres
        self.set_temperature = set_temperature
        self.status = status

class MachineSchema(ma.Schema):
    class Meta:
        # Fields to expose
        fields = ('owner_id','addres', 'set_temperature','status')

machine_schema = MachineSchema()
machines_schema = MachineSchema(many=True)

class Action(db.Model):
    __tablename__ = "Action"
    action_id = db.Column(db.Integer, primary_key=True)
    machine_id = db.Column(db.Integer)
    action_temperature = db.Column(db.Float)


    def __init__(self ,machine_id, action_temperature):
        self.machine_id = machine_id
        self.action_temperature = action_temperature

class ActionSchema(ma.Schema):
    class Meta:
        # Fields to expose
        fields = ('action_id','machine_id','action_temperature')

action_schema = ActionSchema()
actions_schema = ActionSchema(many=True)

@app.route("/actions", methods=["GET"])
def get_all_actions():
    Actions_many = Action.query.all()
    result = actions_schema.dump(Actions_many)

    return jsonify(result)

@app.route("/action/<id>", methods=["GET"])
def get_action(id):
    action = Action.query.get(id)
    result = action_schema.dump(action)

    return jsonify(result)

@app.route("/action/last", methods=["GET"])
def get_last_action():
    all_actions = Action.query.all()
    last_action = all_actions[-1]
    result = action_schema.dump(last_action)

    return jsonify(result)


@app.route("/action/add", methods=["POST"])
def add_action():
    machine_id = request.json["machine_id"]
    action_temperature = request.json["action_temperature"]
    new_action = Action( machine_id, action_temperature)
    db.session.add(new_action)
    db.session.commit()
    action = Action.query.get(new_action.action_id)

    return action_schema.jsonify(action)

@app.route("/machines", methods=["GET"])
def get_all_machines():
    Machines_many = Machine.query.all()
    result = machines_schema.dump(Machines_many)

    return jsonify(result)

@app.route("/machine/<id>", methods=["GET"])
def get_machine(id):
    machine = Machine.query.get(id)
    result = machine_schema.dump(machine)

    return jsonify(result)

@app.route("/machine/add", methods=["POST"])
def add_machine():
    owner_id = request.json["owner_id"]
    addres = request.json["addres"]
    set_temperature = request.json["set_temperature"]
    status = request.json["status"]
    new_machine = Machine(owner_id, addres, set_temperature,status)
    db.session.add(new_machine)
    db.session.commit()
    machine = Machine.query.get(new_machine.id)

    return machine_schema.jsonify(machine)

@app.route('/machine/update/<machine_id>', methods=['PUT'])
def update_machine(machine_id):
    machine = Machine.query.get(machine_id)
    temperature = request.json['set_temperature']
    machine.set_temperature = temperature
    db.session.commit()

    return machine_schema.jsonify(machine)


@app.route("/users", methods=["GET"])
def get_all_users():
    User_many = Users.query.all()
    result = users_schema.dump(User_many)

    return jsonify(result)

@app.route("/users/<id>", methods=["GET"])
def get_user(id):
    User = Users.query.get(id)
    result = user_schema.dump(User)

    return jsonify(result)

@app.route("/users/add", methods=["POST"])
def add_user():
    name = request.json["name"]
    new_user = Users(name)
    db.session.add(new_user)
    db.session.commit()
    user = Users.query.get(new_user.id)

    return user_schema.jsonify(user)

@app.route('/')
def hello_world():
    return 'Podgrzewacz do piłki do squasha, wpisz w adresie /users aby zobaczyć uzytkowników lub /users/id="wybrane_id" aby znaleźć użytkowanika o wybranym id'