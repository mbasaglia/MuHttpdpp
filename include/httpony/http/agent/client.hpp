/**
 * \file
 *
 * \author Mattia Basaglia
 *
 * \copyright Copyright 2016 Mattia Basaglia
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef HTTPONY_HTTP_AGENT_CLIENT_HPP
#define HTTPONY_HTTP_AGENT_CLIENT_HPP

/// \cond
#include <list>

#include <melanolib/utils/movable.hpp>
/// \endcond

#include "httpony/io/basic_client.hpp"
#include "httpony/http/response.hpp"

namespace httpony {

class Client
{
public:
    /**
     * \todo Add more semantic properties to user_agent, and add it as a request attribute
     */
    explicit Client(const std::string& user_agent)
        : _user_agent(user_agent)
    {}

    Client() : Client("HttPony/1.0") {}

    virtual ~Client()
    {}

    /**
     * \brief Creates a connection to the target of the geiven Uri
     * \param[in]  target URI of the server to connect to
     * \param[out] status Status of the operation
     */
    melanolib::Movable<io::Connection> connect(Uri target, ClientStatus& status) const
    {
        if ( target.scheme.empty() )
            target.scheme = "http";

        auto connection = create_connection(target);
        status = _basic_client.connect(target, *connection);

        return connection;
    }

    ClientStatus query(Request& request, Response& response) const
    {
        ClientStatus status;
        auto connection = connect(request.url, status);

        if ( status.error() )
            return status;

        return get_response(connection, request, response);
    }

    ClientStatus get_response(io::Connection& connection, Request& request, Response& response) const;

    /**
     * \brief The timeout for network I/O operations
     */
    melanolib::Optional<melanolib::time::seconds> timeout() const
    {
        return _basic_client.timeout();
    }

    void set_timeout(melanolib::time::seconds timeout)
    {
        _basic_client.set_timeout(timeout);
    }

    void clear_timeout()
    {
        _basic_client.clear_timeout();
    }

protected:

    virtual void process_request(Request& request) const
    {
        request.headers["User-Agent"] = _user_agent;
    }

private:
    /**
     * \brief Creates a new connection object
     */
    virtual melanolib::Movable<io::Connection> create_connection(const Uri& target) const
    {
        /// \todo Handle SSL based on target.scheme
        return melanolib::Movable<io::Connection>(io::SocketTag<io::PlainSocket>{});
    }

    io::BasicClient _basic_client;
    std::string _user_agent;
};

} // namespace httpony
#endif // HTTPONY_HTTP_AGENT_CLIENT_HPP
