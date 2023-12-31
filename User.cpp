
//
// User.cpp
// 2023-03-11
// Ivan Konishchev
//

#include "User.h"
#include "encodepassword.h"

long long int User::_currentId;

User::User(const QString &name, const QString &login, const QString &pass) : _name(name), _login(login)
{
    std::string password = pass.toStdString();
    EncodePassword::sha1(password);
    _pass = QString::fromStdString(password);
}

void User::setUserPassword(const QString &pass)
{
    std::string password = pass.toStdString();
    EncodePassword::sha1(password);
    _pass = QString::fromStdString(password);
}

void User::setUserID(int id)
{
    _id = id;
}

long long User::getId() const
{
    return _id;
}

const QString &User::getUserName() const
{
    return _name;
}

const QString &User::getUserLogin() const
{
    return _login;
}

const QString &User::getUserPassword() const
{
    return _pass;
}

void User::setUserName(const QString &name)
{
    _name = name;
}

void User::setUserLogin(const QString &login)
{
    _login = login;
}

bool User::isAdmin() const
{
    return _isAdmin;
}

bool User::isBanned() const
{
    return _isBanned;
}

bool User::isDeleted() const
{
    return _isDeleted;
}

void User::setIsAdmin(bool isAdmin)
{
    _isAdmin = isAdmin;
}

void User::setIsBanned(bool isBanned)
{
    _isBanned = isBanned;
}

User &User::operator=(const User &user)
{
    _name = user._name;
    _login = user._login;
    _id = user._id;
    _pass = user._pass;
    _isAdmin = user._isAdmin;
    _isBanned = user._isBanned;
    _isDeleted = user._isDeleted;
    return *this;
}


void User::saveUserPassword(const QString &pass)
{
    _pass = pass;
}

QJsonObject User::encodeToJson()
{
    QJsonObject jsonObject;
    jsonObject["name"] = _name;
    jsonObject["login"] = _login;
    jsonObject["password"] = _pass;
    jsonObject["id"] = _id;
    jsonObject["is_admin"] = _isAdmin;
    jsonObject["is_banned"] = _isBanned;
    jsonObject["is_deleted"] = _isDeleted;
    return jsonObject;
}

void User::decodeFromJson(const QJsonObject &data)
{
    _name = data["name"].toString();
    _login = data["login"].toString();
    _pass = data["password"].toString();
    _id = data["id"].toInt();
    _isAdmin = data["is_admin"].toBool();
    _isBanned = data["is_banned"].toBool();
    _isDeleted = data["is_deleted"].toBool();
}
