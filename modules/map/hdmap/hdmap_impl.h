/* Copyright 2017 The Apollo Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
=========================================================================*/

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "modules/common_msgs/map_msgs/map.pb.h"
#include "modules/common_msgs/map_msgs/map_area.pb.h"
#include "modules/common_msgs/map_msgs/map_barrier_gate.pb.h"
#include "modules/common_msgs/map_msgs/map_clear_area.pb.h"
#include "modules/common_msgs/map_msgs/map_crosswalk.pb.h"
#include "modules/common_msgs/map_msgs/map_geometry.pb.h"
#include "modules/common_msgs/map_msgs/map_junction.pb.h"
#include "modules/common_msgs/map_msgs/map_lane.pb.h"
#include "modules/common_msgs/map_msgs/map_overlap.pb.h"
#include "modules/common_msgs/map_msgs/map_parking_space.pb.h"
#include "modules/common_msgs/map_msgs/map_pnc_junction.pb.h"
#include "modules/common_msgs/map_msgs/map_signal.pb.h"
#include "modules/common_msgs/map_msgs/map_speed_bump.pb.h"
#include "modules/common_msgs/map_msgs/map_stop_sign.pb.h"
#include "modules/common_msgs/map_msgs/map_yield_sign.pb.h"

#include "modules/common/math/aabox2d.h"
#include "modules/common/math/aaboxkdtree2d.h"
#include "modules/common/math/line_segment2d.h"
#include "modules/common/math/polygon2d.h"
#include "modules/common/math/vec2d.h"
#include "modules/map/hdmap/hdmap_common.h"

/**
 * @namespace apollo::hdmap
 * @brief apollo::hdmap
 */
namespace apollo {
namespace hdmap {

/**
 * @class HDMapImpl
 *
 * @brief High-precision map loader implement.
 */
class HDMapImpl {
 public:
  using LaneTable = std::unordered_map<std::string, std::shared_ptr<LaneInfo>>;
  using JunctionTable =
      std::unordered_map<std::string, std::shared_ptr<JunctionInfo>>;
  using AreaTable = std::unordered_map<std::string, std::shared_ptr<AreaInfo>>;
  using SignalTable =
      std::unordered_map<std::string, std::shared_ptr<SignalInfo>>;
  using BarrierGateTable =
      std::unordered_map<std::string, std::shared_ptr<BarrierGateInfo>>;
  using CrosswalkTable =
      std::unordered_map<std::string, std::shared_ptr<CrosswalkInfo>>;
  using StopSignTable =
      std::unordered_map<std::string, std::shared_ptr<StopSignInfo>>;
  using YieldSignTable =
      std::unordered_map<std::string, std::shared_ptr<YieldSignInfo>>;
  using ClearAreaTable =
      std::unordered_map<std::string, std::shared_ptr<ClearAreaInfo>>;
  using SpeedBumpTable =
      std::unordered_map<std::string, std::shared_ptr<SpeedBumpInfo>>;
  using OverlapTable =
      std::unordered_map<std::string, std::shared_ptr<OverlapInfo>>;
  using RoadTable = std::unordered_map<std::string, std::shared_ptr<RoadInfo>>;
  using ParkingSpaceTable =
      std::unordered_map<std::string, std::shared_ptr<ParkingSpaceInfo>>;
  using PNCJunctionTable =
      std::unordered_map<std::string, std::shared_ptr<PNCJunctionInfo>>;
  using RSUTable = std::unordered_map<std::string, std::shared_ptr<RSUInfo>>;

 public:
  /**
   * @brief load map from local file
   * @param map_filename path of map data file
   * @return 0:success, otherwise failed
   */
  int LoadMapFromFile(const std::string& map_filename);

  /**
   * @brief load map from a protobuf message
   * @param map_proto map data in protobuf format
   * @return 0:success, otherwise failed
   */
  int LoadMapFromProto(const Map& map_proto);

  LaneInfoConstPtr GetLaneById(const Id& id) const;
  JunctionInfoConstPtr GetJunctionById(const Id& id) const;
  SignalInfoConstPtr GetSignalById(const Id& id) const;
  CrosswalkInfoConstPtr GetCrosswalkById(const Id& id) const;
  StopSignInfoConstPtr GetStopSignById(const Id& id) const;
  YieldSignInfoConstPtr GetYieldSignById(const Id& id) const;
  ClearAreaInfoConstPtr GetClearAreaById(const Id& id) const;
  SpeedBumpInfoConstPtr GetSpeedBumpById(const Id& id) const;
  OverlapInfoConstPtr GetOverlapById(const Id& id) const;
  RoadInfoConstPtr GetRoadById(const Id& id) const;
  ParkingSpaceInfoConstPtr GetParkingSpaceById(const Id& id) const;
  PNCJunctionInfoConstPtr GetPNCJunctionById(const Id& id) const;
  RSUInfoConstPtr GetRSUById(const Id& id) const;
  AreaInfoConstPtr GetAreaById(const Id& id) const;
  BarrierGateInfoConstPtr GetBarrierGateById(const Id& id) const;

  /**
   * @brief convert id data type
   * @param string_id string type of id
   * @return proto type of id
   */
  Id CreateHDMapId(const std::string& string_id) const;
  /**
   * @brief get all areas in certain range
   * @param point the central point of the range
   * @param distance the search radius
   * @param areas store all areas in target range
   * @return 0:success, otherwise failed
   */
  int GetAreas(const apollo::common::PointENU& point, double distance,
               std::vector<AreaInfoConstPtr>* areas) const;
  /**
   * @brief get all lanes in certain range
   * @param point the central point of the range
   * @param distance the search radius
   * @param lanes store all lanes in target range
   * @return 0:success, otherwise failed
   */
  int GetLanes(const apollo::common::PointENU& point, double distance,
               std::vector<LaneInfoConstPtr>* lanes) const;
  /**
   * @brief get all junctions in certain range
   * @param point the central point of the range
   * @param distance the search radius
   * @param junctions store all junctions in target range
   * @return 0:success, otherwise failed
   */
  int GetJunctions(const apollo::common::PointENU& point, double distance,
                   std::vector<JunctionInfoConstPtr>* junctions) const;
  /**
   * @brief get all crosswalks in certain range
   * @param point the central point of the range
   * @param distance the search radius
   * @param crosswalks store all crosswalks in target range
   * @return 0:success, otherwise failed
   */
  int GetCrosswalks(const apollo::common::PointENU& point, double distance,
                    std::vector<CrosswalkInfoConstPtr>* crosswalks) const;
  /**
   * @brief get all signals in certain range
   * @param point the central point of the range
   * @param distance the search radius
   * @param signals store all signals in target range
   * @return 0:success, otherwise failed
   */
  int GetSignals(const apollo::common::PointENU& point, double distance,
                 std::vector<SignalInfoConstPtr>* signals) const;
  /**
   * @brief get all barrier_gates in certain range
   * @param point the central point of the range
   * @param distance the search radius
   * @param barrier_gates store all barrier_gates in target range
   * @return 0:success, otherwise failed
   */
  int GetBarrierGates(const apollo::common::PointENU& point, double distance,
                 std::vector<BarrierGateInfoConstPtr>* barrier_gates) const;
  /**
   * @brief get all stop signs in certain range
   * @param point the central point of the range
   * @param distance the search radius
   * @param stop signs store all stop signs in target range
   * @return 0:success, otherwise failed
   */
  int GetStopSigns(const apollo::common::PointENU& point, double distance,
                   std::vector<StopSignInfoConstPtr>* stop_signs) const;
  /**
   * @brief get all yield signs in certain range
   * @param point the central point of the range
   * @param distance the search radius
   * @param yield signs store all yield signs in target range
   * @return 0:success, otherwise failed
   */
  int GetYieldSigns(const apollo::common::PointENU& point, double distance,
                    std::vector<YieldSignInfoConstPtr>* yield_signs) const;
  /**
   * @brief get all clear areas in certain range
   * @param point the central point of the range
   * @param distance the search radius
   * @param clear_areas store all clear areas in target range
   * @return 0:success, otherwise failed
   */
  int GetClearAreas(const apollo::common::PointENU& point, double distance,
                    std::vector<ClearAreaInfoConstPtr>* clear_areas) const;
  /**
   * @brief get all speed bumps in certain range
   * @param point the central point of the range
   * @param distance the search radius
   * @param speed_bumps store all speed bumps in target range
   * @return 0:success, otherwise failed
   */
  int GetSpeedBumps(const apollo::common::PointENU& point, double distance,
                    std::vector<SpeedBumpInfoConstPtr>* speed_bumps) const;
  /**
   * @brief get all roads in certain range
   * @param point the central point of the range
   * @param distance the search radius
   * @param roads store all roads in target range
   * @return 0:success, otherwise failed
   */
  int GetRoads(const apollo::common::PointENU& point, double distance,
               std::vector<RoadInfoConstPtr>* roads) const;

  /**
   * @brief get all parking space in certain range
   * @param point the central point of the range
   * @param distance the search radius
   * @param parking_spaces store all parking spaces in target range
   * @return 0:success, otherwise failed
   */
  int GetParkingSpaces(
      const apollo::common::PointENU& point, double distance,
      std::vector<ParkingSpaceInfoConstPtr>* parking_spaces) const;

  /**
   * @brief get all pnc junctions in certain range
   * @param point the central point of the range
   * @param distance the search radius
   * @param junctions store all junctions in target range
   * @return 0:success, otherwise failed
   */
  int GetPNCJunctions(
      const apollo::common::PointENU& point, double distance,
      std::vector<PNCJunctionInfoConstPtr>* pnc_junctions) const;

  /**
   * @brief get nearest lane from target point with search radius,
   * @param point the target point
   * @param distance the search radius
   * @param nearest_lane the nearest lane that match search conditions
   * @param nearest_s the offset from lane start point along lane center line
   * @param nearest_l the lateral offset from lane center line
   * @return 0:success, otherwise, failed.
   */
  int GetNearestLaneWithDistance(const apollo::common::PointENU& point,
                                 const double distance,
                                 LaneInfoConstPtr* nearest_lane,
                                 double* nearest_s, double* nearest_l) const;

  /**
   * @brief get nearest lane from target point,
   * @param point the target point
   * @param nearest_lane the nearest lane that match search conditions
   * @param nearest_s the offset from lane start point along lane center line
   * @param nearest_l the lateral offset from lane center line
   * @return 0:success, otherwise, failed.
   */
  int GetNearestLane(const apollo::common::PointENU& point,
                     LaneInfoConstPtr* nearest_lane, double* nearest_s,
                     double* nearest_l) const;
  /**
   * @brief get the nearest lane within a certain range by pose
   * @param point the target position
   * @param distance the search radius
   * @param central_heading the base heading
   * @param max_heading_difference the heading range
   * @param nearest_lane the nearest lane that match search conditions
   * @param nearest_s the offset from lane start point along lane center line
   * @param nearest_l the lateral offset from lane center line
   * @return 0:success, otherwise, failed.
   */
  int GetNearestLaneWithHeading(const apollo::common::PointENU& point,
                                const double distance,
                                const double central_heading,
                                const double max_heading_difference,
                                LaneInfoConstPtr* nearest_lane,
                                double* nearest_s, double* nearest_l) const;
  /**
   * @brief get all lanes within a certain range by pose
   * @param point the target position
   * @param distance the search radius
   * @param central_heading the base heading
   * @param max_heading_difference the heading range
   * @param nearest_lane all lanes that match search conditions
   * @return 0:success, otherwise, failed.
   */
  int GetLanesWithHeading(const apollo::common::PointENU& point,
                          const double distance, const double central_heading,
                          const double max_heading_difference,
                          std::vector<LaneInfoConstPtr>* lanes) const;
  /**
   * @brief get all road and junctions boundaries within certain range
   * @param point the target position
   * @param radius the search radius
   * @param road_boundaries the roads' boundaries
   * @param junctions the junctions' boundaries
   * @return 0:success, otherwise failed
   */
  int GetRoadBoundaries(const apollo::common::PointENU& point, double radius,
                        std::vector<RoadROIBoundaryPtr>* road_boundaries,
                        std::vector<JunctionBoundaryPtr>* junctions) const;
  /**
   * @brief get all road boundaries and junctions within certain range
   * @param point the target position
   * @param radius the search radius
   * @param road_boundaries the roads' boundaries
   * @param junctions the junctions
   * @return 0:success, otherwise failed
   */
  int GetRoadBoundaries(const apollo::common::PointENU& point, double radius,
                        std::vector<RoadRoiPtr>* road_boundaries,
                        std::vector<JunctionInfoConstPtr>* junctions) const;
  /**
   * @brief get ROI within certain range
   * @param point the target position
   * @param radius the search radius
   * @param roads_roi the roads' boundaries
   * @param polygons_roi the junctions' boundaries
   * @return 0:success, otherwise failed
   */
  int GetRoi(const apollo::common::PointENU& point, double radius,
             std::vector<RoadRoiPtr>* roads_roi,
             std::vector<PolygonRoiPtr>* polygons_roi);
  /**
   * @brief get forward nearest signals within certain range on the lane
   *        if there are two signals related to one stop line,
   *        return both signals.
   * @param point the target position
   * @param distance the forward search distance
   * @param signals all signals match conditions
   * @return 0:success, otherwise failed
   */
  int GetForwardNearestSignalsOnLane(
      const apollo::common::PointENU& point, const double distance,
      std::vector<SignalInfoConstPtr>* signals) const;

  /**
   * @brief get forward nearest barrier_gates within certain range on the lane
   * @param point the target position
   * @param distance the forward search distance
   * @param barrier_gates all barrier_gates match conditions
   * @return 0:success, otherwise failed
   */
  int GetForwardNearestBarriersOnLane(
      const apollo::common::PointENU& point, const double distance,
      std::vector<BarrierGateInfoConstPtr>* barrier_gates) const;

  /**
   * @brief get all other stop signs associated with a stop sign
   *        in the same junction
   * @param id id of stop sign
   * @param stop_signs stop signs associated
   * @return 0:success, otherwise failed
   */
  int GetStopSignAssociatedStopSigns(
      const Id& id, std::vector<StopSignInfoConstPtr>* stop_signs) const;

  /**
   * @brief get all lanes associated with a stop sign in the same junction
   * @param id id of stop sign
   * @param lanes all lanes match conditions
   * @return 0:success, otherwise failed
   */
  int GetStopSignAssociatedLanes(const Id& id,
                                 std::vector<LaneInfoConstPtr>* lanes) const;

  /**
   * @brief get a local map which is identical to the origin map except that all
   * map elements without overlap with the given region are deleted.
   * @param point the target position
   * @param range the size of local map region, [width, height]
   * @param local_map local map in proto format
   * @return 0:success, otherwise failed
   */
  int GetLocalMap(const apollo::common::PointENU& point,
                  const std::pair<double, double>& range, Map* local_map) const;

  /**
   * @brief get forward nearest rsus within certain range
   * @param point the target position
   * @param distance the forward search distance
   * @param central_heading the base heading
   * @param max_heading_difference the heading range
   * @param rsus all rsus that match search conditions
   * @return 0:success, otherwise failed
   */
  int GetForwardNearestRSUs(const apollo::common::PointENU& point,
                            double distance, double central_heading,
                            double max_heading_difference,
                            std::vector<RSUInfoConstPtr>* rsus) const;

  bool GetMapHeader(Header* map_header) const;

 private:
  int GetLanes(const apollo::common::math::Vec2d& point, double distance,
               std::vector<LaneInfoConstPtr>* lanes) const;
  int GetJunctions(const apollo::common::math::Vec2d& point, double distance,
                   std::vector<JunctionInfoConstPtr>* junctions) const;
  int GetAreas(const apollo::common::math::Vec2d&, double distance,
               std::vector<AreaInfoConstPtr>* areas) const;
  int GetCrosswalks(const apollo::common::math::Vec2d& point, double distance,
                    std::vector<CrosswalkInfoConstPtr>* crosswalks) const;
  int GetSignals(const apollo::common::math::Vec2d& point, double distance,
                 std::vector<SignalInfoConstPtr>* signals) const;
  int GetBarrierGates(const apollo::common::math::Vec2d& point, double distance,
                 std::vector<BarrierGateInfoConstPtr>* barrier_gates) const;
  int GetStopSigns(const apollo::common::math::Vec2d& point, double distance,
                   std::vector<StopSignInfoConstPtr>* stop_signs) const;
  int GetYieldSigns(const apollo::common::math::Vec2d& point, double distance,
                    std::vector<YieldSignInfoConstPtr>* yield_signs) const;
  int GetClearAreas(const apollo::common::math::Vec2d& point, double distance,
                    std::vector<ClearAreaInfoConstPtr>* clear_areas) const;
  int GetSpeedBumps(const apollo::common::math::Vec2d& point, double distance,
                    std::vector<SpeedBumpInfoConstPtr>* speed_bumps) const;
  int GetParkingSpaces(
      const apollo::common::math::Vec2d& point, double distance,
      std::vector<ParkingSpaceInfoConstPtr>* parking_spaces) const;
  int GetPNCJunctions(
      const apollo::common::math::Vec2d& point, double distance,
      std::vector<PNCJunctionInfoConstPtr>* pnc_junctions) const;
  int GetNearestLane(const apollo::common::math::Vec2d& point,
                     LaneInfoConstPtr* nearest_lane, double* nearest_s,
                     double* nearest_l) const;
  int GetNearestLaneWithHeading(const apollo::common::math::Vec2d& point,
                                const double distance,
                                const double central_heading,
                                const double max_heading_difference,
                                LaneInfoConstPtr* nearest_lane,
                                double* nearest_s, double* nearest_l) const;
  int GetLanesWithHeading(const apollo::common::math::Vec2d& point,
                          const double distance, const double central_heading,
                          const double max_heading_difference,
                          std::vector<LaneInfoConstPtr>* lanes) const;
  int GetRoads(const apollo::common::math::Vec2d& point, double distance,
               std::vector<RoadInfoConstPtr>* roads) const;
  int GetNearestLaneWithDistance(const apollo::common::math::Vec2d& point,
                                 const double distance,
                                 LaneInfoConstPtr* nearest_lane,
                                 double* nearest_s, double* nearest_l) const;

  template <class Table, class BoxTable, class KDTree>
  static void BuildSegmentKDTree(
      const Table& table, const apollo::common::math::AABoxKDTreeParams& params,
      BoxTable* const box_table, std::unique_ptr<KDTree>* const kdtree);

  template <class Table, class BoxTable, class KDTree>
  static void BuildPolygonKDTree(
      const Table& table, const apollo::common::math::AABoxKDTreeParams& params,
      BoxTable* const box_table, std::unique_ptr<KDTree>* const kdtree);

  void BuildLaneSegmentKDTree();
  void BuildJunctionPolygonKDTree();
  void BuildCrosswalkPolygonKDTree();
  void BuildSignalSegmentKDTree();
  void BuildStopSignSegmentKDTree();
  void BuildYieldSignSegmentKDTree();
  void BuildClearAreaPolygonKDTree();
  void BuildSpeedBumpSegmentKDTree();
  void BuildParkingSpacePolygonKDTree();
  void BuildPNCJunctionPolygonKDTree();
  void BuildAreaPolygonKDTree();
  void BuildBarrierGateSegmentKDTree();

  template <class KDTree>
  static int SearchObjects(const apollo::common::math::Vec2d& center,
                           const double radius, const KDTree& kdtree,
                           std::vector<std::string>* const results);

  void Clear();

 private:
  Map map_;
  LaneTable lane_table_;
  JunctionTable junction_table_;
  AreaTable area_table_;
  CrosswalkTable crosswalk_table_;
  SignalTable signal_table_;
  BarrierGateTable barrier_gate_table_;
  StopSignTable stop_sign_table_;
  YieldSignTable yield_sign_table_;
  ClearAreaTable clear_area_table_;
  SpeedBumpTable speed_bump_table_;
  OverlapTable overlap_table_;
  RoadTable road_table_;
  ParkingSpaceTable parking_space_table_;
  PNCJunctionTable pnc_junction_table_;
  RSUTable rsu_table_;

  std::vector<LaneSegmentBox> lane_segment_boxes_;
  std::unique_ptr<LaneSegmentKDTree> lane_segment_kdtree_;

  std::vector<JunctionPolygonBox> junction_polygon_boxes_;
  std::unique_ptr<JunctionPolygonKDTree> junction_polygon_kdtree_;

  std::vector<AreaPolygonBox> area_polygon_boxes_;
  std::unique_ptr<AreaPolygonKDTree> area_polygon_kdtree_;

  std::vector<CrosswalkPolygonBox> crosswalk_polygon_boxes_;
  std::unique_ptr<CrosswalkPolygonKDTree> crosswalk_polygon_kdtree_;

  std::vector<SignalSegmentBox> signal_segment_boxes_;
  std::unique_ptr<SignalSegmentKDTree> signal_segment_kdtree_;

  std::vector<BarrierGateSegmentBox> barrier_gate_segment_boxes_;
  std::unique_ptr<BarrierGateSegmentKDTree> barrier_gate_segment_kdtree_;

  std::vector<StopSignSegmentBox> stop_sign_segment_boxes_;
  std::unique_ptr<StopSignSegmentKDTree> stop_sign_segment_kdtree_;

  std::vector<YieldSignSegmentBox> yield_sign_segment_boxes_;
  std::unique_ptr<YieldSignSegmentKDTree> yield_sign_segment_kdtree_;

  std::vector<ClearAreaPolygonBox> clear_area_polygon_boxes_;
  std::unique_ptr<ClearAreaPolygonKDTree> clear_area_polygon_kdtree_;

  std::vector<SpeedBumpSegmentBox> speed_bump_segment_boxes_;
  std::unique_ptr<SpeedBumpSegmentKDTree> speed_bump_segment_kdtree_;

  std::vector<ParkingSpacePolygonBox> parking_space_polygon_boxes_;
  std::unique_ptr<ParkingSpacePolygonKDTree> parking_space_polygon_kdtree_;

  std::vector<PNCJunctionPolygonBox> pnc_junction_polygon_boxes_;
  std::unique_ptr<PNCJunctionPolygonKDTree> pnc_junction_polygon_kdtree_;
};

}  // namespace hdmap
}  // namespace apollo
