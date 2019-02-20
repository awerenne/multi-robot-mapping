"""
    Description.
"""



#---------------
class Environment():
    """
    Contains maze - generates maze - interface with robots (feedback sensor). 
    interface with master (groundtruth)
    """

    def __init__(self):
        self._ground_truth = []

    @property
    def ground_truth(self):
        return self._ground_truth
    
    def new(self):
        return self



        













    