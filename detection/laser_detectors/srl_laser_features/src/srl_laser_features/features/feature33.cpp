/*
* Software License Agreement (BSD License)
*
*  Copyright (c) 2014-2015, Timm Linder, Social Robotics Lab, University of Freiburg
*  Copyright (c) 2006-2012, Matthias Luber, Luciano Spinello and Kai O. Arras, Social Robotics Laboratory and
*    Oscar Martinez, Autonomous Intelligent Systems, University of Freiburg
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions are met:
*
*  * Redistributions of source code must retain the above copyright notice, this
*    list of conditions and the following disclaimer.
*  * Redistributions in binary form must reproduce the above copyright notice,
*    this list of conditions and the following disclaimer in the documentation
*    and/or other materials provided with the distribution.
*  * Neither the name of the copyright holder nor the names of its contributors
*    may be used to endorse or promote products derived from this software
*    without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
*  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
*  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
*  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
*  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
*  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
*  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <srl_laser_features/features/feature33.h>
#include <cmath>

namespace srl_laser_features {

void Feature33::evaluate(const Segment& segment, Eigen::VectorXd& result) const
{
	result = Eigen::Vector2d();

	// distance between first point of segment and preceding point
	if( std::isnan(segment.precedingPoint(0)) || std::isnan(segment.precedingPoint(1)) ) {
		// No preceding point exists
		result(0) = -1.0;
	}
	else {
		result(0) = (segment.points.front() - segment.precedingPoint).norm();
	}

	// distance between last point of segment and succeeding point
	if( std::isnan(segment.succeedingPoint(0)) || std::isnan(segment.succeedingPoint(1)) ) {
		// No succeeding point exists
		result(1) = -1.0;
	}
	else {
		result(1) = (segment.points.back() - segment.succeedingPoint).norm();
	}
}

} // end of namespace srl_laser_features
