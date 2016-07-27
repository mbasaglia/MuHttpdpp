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

#include "httpony/http/request.hpp"
#include "httpony/http/post/post.hpp"

namespace httpony {

bool Request::can_parse_post() const
{
    return body.has_data() &&
        httpony::post::FormatRegistry::instance().can_parse(body.content_type());
}

bool Request::parse_post()
{
    if ( !body.has_data() )
        return false;

    return httpony::post::FormatRegistry::instance().parse(*this);
}

} // namespace httpony
