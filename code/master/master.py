"""
    Description.
"""

from map import Map
import threading 


# TODO: bi-directional queue, more elegant (very low priority)
# TODO: proper thread/general quitting (very low priority)


# ------------
class Master:
    """
    Blabla.
    """

    def __init__(self, queu_to_gui, queu_from_gui):
        self.map = Map()
        self.run_robots = True

        self.queu_to_gui = queu_to_gui
        self.queu_from_gui = queu_from_gui

        self.queu_to_messenger = Queue()
        self.queu_from_messenger =  Queue()
        self.messenger = Messenger(self.queu_from_messenger,
                self.queu_to_messenger)

    # ------------
    def run():
        while True:
            # Check if new message from robot
            if not self.queu_from_messenger:
                directive = self.queu_from_messenger.get()
                if directive.type_directive == "update":
                    # Update map
                    directive = self.map.update(directive)
                    self.queu_to_gui.put(directive)

                    # Make decision and send instruction to robot
                    instruction = self.make_decision()
                    directive = self.make_directive(instruction)
                    self.queu_to_messenger.put(directive)

                    # Update GUI
                    directive_to_gui.instruction = instruction
                    self.queu_to_gui.put(directive_to_gui)

            # Check if new user interaction
            if not self.queu_from_gui:
                directive = self.queu_from_gui.get()
                if directive.type_directive == "parameter_update":
                    self.run_robots = directive["run"]
                    # TODO: update other parameters

    # ------------
    def make_decision(self):
        if self.map.is_robot_at_frontier(id_robot):
            directions = self.map.unexplored_directions(id_robot)
            if id_robot in self.targets:
                del self.targets[id_robot]

            # Left Hand Rule
            if directions["left"] in directions:
                return "left"
            if directions["front"] in directions:
                return "front"
            if directions["right"] in directions:
                return "right"
            else:
                return "stop"

        # Robot is already on the way to previously assigned frontier
        if id_robot in self.targets:
            (directions, _) = self.map.shortest_path(self.robot_position(id_robot),
                    self.targets[id_robot])
            return directions[0]

        # Assign new frontier to robot, and direct robot towards this target
        frontiers = self.map.frontiers
        nearest_frontier = self.map.nearest_frontier()
        (directions, _) = self.map.shortest_path(self.robot_position(id_robot),
                    nearest_frontier)
        self.targets[id_robot] = nearest_frontier
        return directions[0]


    # ------------
    def make_directive(self, type_directive, instruction):
        if type_directive == "new_message":
            directive = {
                "type_directive": type_directive,
                "id_robot": id_robot,
                "instruction": instruction
            }
        elif type_directive == "update_direction_gui":
            directive = {
                "type_directive": type_directive,
                "id_robot": id_robot,
                "instruction": instruction
            }
        return Container(directive)

















    