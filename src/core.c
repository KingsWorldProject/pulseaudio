#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "core.h"
#include "module.h"
#include "sink.h"
#include "source.h"

struct core* core_new(struct mainloop *m) {
    struct core* c;
    c = malloc(sizeof(struct core));
    assert(c);

    c->mainloop = m;
    c->clients = idxset_new(NULL, NULL);
    c->sinks = idxset_new(NULL, NULL);
    c->sources = idxset_new(NULL, NULL);
    c->output_streams = idxset_new(NULL, NULL);
    c->input_streams = idxset_new(NULL, NULL);

    c->default_source_index = c->default_sink_index = IDXSET_INVALID;

    c->modules = NULL;
    
    return c;
};

void core_free(struct core *c) {
    assert(c);

    module_unload_all(c);
    assert(!c->modules);
    
    assert(idxset_isempty(c->clients));
    idxset_free(c->clients, NULL, NULL);
    
    assert(idxset_isempty(c->sinks));
    idxset_free(c->sinks, NULL, NULL);

    assert(idxset_isempty(c->sources));
    idxset_free(c->sources, NULL, NULL);
    
    assert(idxset_isempty(c->output_streams));
    idxset_free(c->output_streams, NULL, NULL);
    
    assert(idxset_isempty(c->input_streams));
    idxset_free(c->input_streams, NULL, NULL);

    free(c);    
};

struct sink* core_get_default_sink(struct core *c) {
    struct sink *sink;
    assert(c);

    if ((sink = idxset_get_by_index(c->sinks, c->default_sink_index)))
        return sink;

    if (!(sink = idxset_first(c->sinks, &c->default_sink_index)))
        return NULL;

    fprintf(stderr, "core: default sink vanished, setting to %u.\n", sink->index);
    return sink;
}

struct source* core_get_default_source(struct core *c) {
    struct source *source;
    assert(c);

    if ((source = idxset_get_by_index(c->sources, c->default_source_index)))
        return source;

    if (!(source = idxset_first(c->sources, &c->default_source_index)))
        return NULL;

    fprintf(stderr, "core: default source vanished, setting to %u.\n", source->index);
    return source;
}
